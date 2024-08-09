//
// Created by Keegan Beaulieu on 2024-07-21.
//

#include "UiViewport.h"

List<UiViewport *> UiViewport::viewports;
std::unordered_map<unsigned int, Material *> UiViewport::gizmo_mouse_test_materials;
unsigned int UiViewport::gizmo_mouse_test_fragment;

void UiViewport::draw_gizmo(Mesh *mesh, Material *material, Transform *transform, GizmoCallbacks callbacks)
{
    gizmos_target->bind();

    material->set("mvp", transform->build_trs()*camera->build_vp_matrix());
    material->bind();
    mesh->draw();

    Material *test_material = nullptr;

    try {
        test_material = gizmo_mouse_test_materials.at(material->shader->vertex);
    } catch (...) {
        Shader *test_shader = Shader::create(material->shader->vertex, gizmo_mouse_test_fragment);
        test_material = Material::create(test_shader);
        gizmo_mouse_test_materials[material->shader->vertex] = test_material;
    }

    gizmo_callbacks.push_back(callbacks);
    unsigned int callbacks_index = gizmo_callbacks.size();

    gizmos_mouse_test_target->bind();

    test_material->set("mvp", transform->build_trs()*camera->build_vp_matrix());
    test_material->set("callbacks", callbacks_index);
    test_material->bind();
    mesh->draw();
}

void UiViewport::init()
{
    gizmo_mouse_test_fragment = Shader::compile("gizmo_mouse_test.glsl", Shader::SHADER_FRAGMENT);
}

UiViewport *UiViewport::create(UiContainer *parent)
{
    UiViewport *viewport = alloc<UiViewport>(parent);

    viewport->main_target = RenderTarget::create({
        .width = 1,
        .height = 1,
        .name = "Viewport Render Target"
    });

#ifdef __EDITOR__
    viewport->gizmos_target = RenderTarget::create({
        .width = 1,
        .height = 1,
        .name = "Viewport Gizmos Render Target"
    });

    viewport->gizmos_mouse_test_target = RenderTarget::create({
        .width = 1,
        .height = 1,
        .name = "Viewport Mouse Test Render Target",
        .format = Texture::TEX_32UI
    });
#endif

    viewport->on_update.subscribe(&handle_update, nullptr);
    viewport->on_mouse_move.subscribe(&handle_mouse_move, nullptr);
    viewport->on_click.subscribe(&handle_click, nullptr);
    viewport->off_click.subscribe(&handle_off_click, nullptr);
    viewport->layers = std::vector<UiContainer *>();
    viewport->gizmo_callbacks = std::vector<GizmoCallbacks>();

    UiContainer *main_layer = UiContainer::create(viewport);
    main_layer->positioning = UI_RELATIVE;
    main_layer->size = viewport->calculated_size;
    main_layer->texture = viewport->main_target->texture;
    main_layer->depth = 1;
    viewport->layers.push_back(main_layer);

#ifdef __EDITOR__
    UiContainer *gizmos_layer = UiContainer::create(viewport);
    gizmos_layer->positioning = UI_RELATIVE;
    gizmos_layer->size = viewport->calculated_size;
    gizmos_layer->texture = viewport->gizmos_target->texture;
    gizmos_layer->depth = 2;
    viewport->layers.push_back(gizmos_layer);
#endif

    viewports.alloc(viewport);

    return viewport;
}

void UiViewport::handle_update(UiContainer *container, void *)
{
    UiViewport *viewport = (UiViewport *)container;
    viewport->main_target->resize(viewport->calculated_size.x, viewport->calculated_size.y);

#ifdef __EDITOR__
    viewport->gizmos_target->resize(viewport->calculated_size.x, viewport->calculated_size.y);
    viewport->gizmos_mouse_test_target->resize(viewport->calculated_size.x, viewport->calculated_size.y);
#endif

    for (auto layer : viewport->layers) {
        layer->size = viewport->calculated_size;
        layer->calculated_size = viewport->calculated_size;
    }
}

void UiViewport::handle_destroy(UiContainer *container, void *)
{
    UiViewport *viewport = (UiViewport *)container;
    viewport->layers.~vector();
    viewport->gizmo_callbacks.~vector();
}

void UiViewport::clear_all()
{
    for (auto viewport : viewports) {
        viewport->main_target->clear();

#ifdef __EDITOR__
        viewport->gizmo_callbacks.clear();
        viewport->gizmos_target->clear();
        viewport->gizmos_mouse_test_target->clear();
#endif
    }
}

void UiViewport::handle_mouse_move(MouseEvent event, void *)
{

}

void UiViewport::handle_click(MouseEvent event, void *)
{
    UiViewport *viewport = (UiViewport *)event.container;
    int id = viewport->get_gizmo_callbacks_id(event);

    if (id != -1) {
        GizmoCallbacks callbacks = viewport->gizmo_callbacks[id];

        if (callbacks.on_click)
            callbacks.on_click(callbacks.data);
        viewport->current_gizmo_off_click = callbacks.off_click;
        viewport->current_gizmo_off_click_data = callbacks.data;
    }
}

void UiViewport::handle_off_click(MouseEvent event, void *)
{
    UiViewport *viewport = (UiViewport *)event.container;

    if (viewport->current_gizmo_off_click) {
        viewport->current_gizmo_off_click(viewport->current_gizmo_off_click_data);
        viewport->current_gizmo_off_click = nullptr;
    }
}

int UiViewport::get_gizmo_callbacks_id(MouseEvent event)
{
    Vector2i position = event.position - calculated_position;
    return (int)gizmos_mouse_test_target->read_pixel<unsigned int>(position.x, position.y) - 1;
}
