//
// Created by Keegan Beaulieu on 2024-07-27.
//

#include "ContextEditor.h"

#include "ContextExplorer.h"
#include "EntityInspector.h"
#include "EntityInspector.h"
#include "UiViewport.h"
#include "ExplorerEntity.h"

ContextEditor *ContextEditor::current = nullptr;

void ContextEditor::select_entity(Entity *entity)
{
    inspector->load_entity(entity);
    selected = entity;
}

Entity *ContextEditor::create_entity()
{
    Entity *entity = Entity::create("New Entity", nullptr);
    ExplorerEntity::create(explorer, entity);
    explorer->update();

    return entity;
}

void ContextEditor::draw_gizmos()
{
    if (!selected)
        return;

    float camera_dist = (viewport->camera->transform->position - selected->transform->position).length();

    Transform transform;
    transform.position = selected->transform->position;
    transform.scale = Vector3f(1.0f, 1.0f, 1.0f);

    transform.rotation = Vector3f(0, 0, M_PI_2);
    Material::defaults.flat->set("colour", Colour(1.0f, 0.0f, 0.0f, 1.0f));
    viewport->draw_gizmo(Mesh::defaults.arrow, Material::defaults.flat, &transform, {
        .on_click = [](void *data) {
            ContextEditor *editor = (ContextEditor *)data;

            editor->transformation_axes = EDITOR_AXIS_X;
            editor->transformation = EDITOR_TRANS_TRANSLATION;
        },
        .off_click = [](void *data) {
            ContextEditor *editor = (ContextEditor *)data;

            editor->transformation_axes = EDITOR_AXIS_NONE;
            editor->transformation = EDITOR_TRANS_NONE;
        },
        .data = this
    });

    transform.rotation = Vector3f(0, 0, 0);
    Material::defaults.flat->set("colour", Colour(0.0f, 0.0f, 1.0f, 1.0f));
    viewport->draw_gizmo(Mesh::defaults.arrow, Material::defaults.flat, &transform, {
        .on_click = [](void *data) {
            ContextEditor *editor = (ContextEditor *)data;

            editor->transformation_axes = EDITOR_AXIS_Y;
            editor->transformation = EDITOR_TRANS_TRANSLATION;
        },
        .off_click = [](void *data) {
            ContextEditor *editor = (ContextEditor *)data;

            editor->transformation_axes = EDITOR_AXIS_NONE;
            editor->transformation = EDITOR_TRANS_NONE;
        },
        .data = this
    });

    transform.rotation = Vector3f(-M_PI_2, 0, 0);
    Material::defaults.flat->set("colour", Colour(0.0f, 1.0f, 0.0f, 1.0f));
    viewport->draw_gizmo(Mesh::defaults.arrow, Material::defaults.flat, &transform, {
        .on_click = [](void *data) {
            ContextEditor *editor = (ContextEditor *)data;

            editor->transformation_axes = EDITOR_AXIS_Z;
            editor->transformation = EDITOR_TRANS_TRANSLATION;
        },
        .off_click = [](void *data) {
            ContextEditor *editor = (ContextEditor *)data;

            editor->transformation_axes = EDITOR_AXIS_NONE;
            editor->transformation = EDITOR_TRANS_NONE;
        },
        .data = this
    });
}

ContextEditor *ContextEditor::create(UiContainer *parent)
{
    ContextEditor *editor = alloc<ContextEditor>(parent);

    editor->layout = UI_RIGHT;
    editor->size = Vector2i(UI_SIZE_GROW, UI_SIZE_GROW);
    editor->on_destroy.subscribe(&handle_destroy, nullptr);
    editor->explorer = ContextExplorer::create(editor);
    editor->viewport = UiViewport::create(editor);
    editor->viewport->size.x = UI_SIZE_GROW;
    editor->viewport->size.y = UI_SIZE_GROW;
    editor->inspector = EntityInspector::create(editor, editor->explorer);
    editor->update();

    Entity *camera_entity = Entity::create("Viewport Camera", nullptr);
    CCamera *camera = camera_entity->add_component<CCamera>();
    camera->projection = CCamera::PERSPECTIVE;
    camera->transform->position = Vector3f(5, 2, 5);
    camera->transform->rotation = Vector3f(M_PI_4/2, -M_PI_4, 0);
    camera->target = editor->viewport->main_target;
    editor->viewport->camera = camera;

    editor->mouse_move_subscription = Window::on_mouse_move.subscribe(&handle_mouse_move, editor);

    return editor;
}

void ContextEditor::handle_mouse_move(Window::MouseEvent event, void *data)
{
    ContextEditor *editor = (ContextEditor *)data;
    if (editor->transformation == EDITOR_TRANS_NONE)
        return;

    Vector2i screen = event.position - editor->viewport->calculated_position;
    CCamera::Ray ray = editor->viewport->camera->ray(Vector2i(screen.x, screen.y));

    switch (editor->transformation_axes) {
        case EDITOR_AXIS_X: {
            float multiplier = -ray.origin.y/ray.direction.y;
            editor->selected->transform->position.x = (ray.origin + ray.direction*multiplier).x;
        } break;
        case EDITOR_AXIS_Z: {
            float multiplier = -ray.origin.y/ray.direction.y;
            editor->selected->transform->position.z = (ray.origin + ray.direction*multiplier).z;
        } break;
        case EDITOR_AXIS_Y: {
            Vector3f camera_rot_y = Vector3f(0, editor->viewport->camera->transform->rotation.y, 0);
            Mat4x4f camera_rot_y_m = Mat4x4f::rotation(camera_rot_y);

            ray.origin = camera_rot_y_m*ray.origin;
            ray.direction = camera_rot_y_m*ray.direction;

            float multiplier = -ray.origin.x/ray.direction.x;
            editor->selected->transform->position.y = (ray.origin + ray.direction*multiplier).y;
        } break;
    }

    editor->select_entity(editor->selected);
}

void ContextEditor::handle_destroy(UiContainer *container, void *)
{
    ContextEditor *editor = (ContextEditor *)container;
    Window::on_mouse_move.unsubscribe(editor->mouse_move_subscription);
}
