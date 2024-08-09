//
// Created by Keegan Beaulieu on 2024-07-08.
//

#include "UiContainer.h"

#include <UiBorder.h>

#include "Material.h"
#include "UiStyle.h"
#include "Window.h"
#include "UiShadow.h"

UiContainer *UiContainer::root_container = nullptr;
List<UiContainer> UiContainer::containers;
std::set<UiContainer *> UiContainer::non_static_containers;
std::unordered_map<UiContainer *, UiContainer::RenderTargetPair> UiContainer::shadow_textures;
UiContainer *UiContainer::hovered = nullptr;
UiContainer *UiContainer::focused = nullptr;
UiContainer *UiContainer::clicked = nullptr;
Event<UiContainer::MouseEvent> UiContainer::static_on_click;
List<UiContainer::DeferredCallback> UiContainer::deferred_callbacks;

void UiContainer::update()
{
    if (!parent) {
        for (auto &child : children) {
            child->calculate_non_grow_size();
        }

        calculate_children_grow_sizes();
        position_children();

        return;
    }

    if (parent->size.x == UI_SIZE_FIT || parent->size.y == UI_SIZE_FIT) {
        parent->update();
    } else {
        for (auto &child : parent->children) {
            child->calculate_non_grow_size();
        }

        parent->calculate_children_grow_sizes();
        parent->position_children();
    }
}

UiContainer *UiContainer::create(UiContainer *parent)
{
    return alloc<UiContainer>(parent);
}

void UiContainer::cleanup_recursive(UiContainer *container)
{
    for (auto child : container->children) {
        cleanup_recursive(child);
    }

    container->on_destroy.fire(container);

    container->children.~vector();
    container->style_tags.~vector();
    Event<UiContainer *>::destroy(container->on_render);
    Event<UiContainer *>::destroy(container->on_update);
    Event<UiContainer *>::destroy(container->on_destroy);
    Event<UiContainer *>::destroy(container->on_focus);
    Event<UiContainer *>::destroy(container->off_focus);
    Event<MouseEvent>::destroy(container->on_click);
    Event<MouseEvent>::destroy(container->on_hover);
    Event<MouseEvent>::destroy(container->off_click);
    Event<MouseEvent>::destroy(container->off_hover);
    Event<MouseEvent>::destroy(container->on_mouse_move);
    Event<KeyEvent>::destroy(container->on_key_down);
    Event<KeyEvent>::destroy(container->on_key_up);
    Event<KeyEvent>::destroy(container->on_key_repeat);

    if (non_static_containers.contains(container))
        non_static_containers.erase(container);

    if (shadow_textures.contains(container))
        shadow_textures.erase(container);

    if (hovered == container)
        hovered = nullptr;
    if (clicked == container)
        clicked = nullptr;
    if (focused == container)
        focused = nullptr;

    containers.destroy(container);
}

void UiContainer::destroy(UiContainer *container)
{
    if (container->parent) {
        for (int i = 0; i < container->parent->children.size(); ++i) {
            if (container->parent->children[i] == container) {
                container->parent->children.erase(container->parent->children.begin() + i);
                break;
            }
        }
    }

    cleanup_recursive(container);
}

void UiContainer::defer(void(*callback)(void *), void *data)
{
    deferred_callbacks.alloc({ callback, data });
}

void UiContainer::fire_deferred()
{
    for (auto callback : deferred_callbacks) {
        callback.callback(callback.data);
    }

    deferred_callbacks.clear();
}

void UiContainer::render_tree()
{
    root_container->render();
}

void UiContainer::init()
{
    root_container = create(nullptr);
    root_container->calculated_size = Window::true_size/PIXEL_SCALE;

    Window::on_resize.subscribe([](Vector2i size, void *) -> void {
        root_container->calculated_size = size/PIXEL_SCALE;

        for (auto child : root_container->children) {
            child->calculate_non_grow_size();
        }

        root_container->calculate_children_grow_sizes();
        root_container->position_children();
    }, nullptr);

    Window::on_click.subscribe([](Window::MouseEvent event, void *) -> void {
        propagate_mouse_event(event);
    }, nullptr);

    Window::off_click.subscribe([](Window::MouseEvent event, void *) -> void {
        propagate_mouse_event(event);
    }, nullptr);

    Window::on_mouse_move.subscribe([](Window::MouseEvent event, void *) -> void {
        propagate_mouse_event(event);
    }, nullptr);

    Window::on_key_down.subscribe([](Window::KeyEvent event, void *) -> void {
        if (focused)
            focused->on_key_down.fire({ KeyEvent::UI_KEY_DOWN, event.vk, focused });
    }, nullptr);

    Window::on_key_up.subscribe([](Window::KeyEvent event, void *) -> void {
        if (focused)
            focused->on_key_up.fire({ KeyEvent::UI_KEY_UP, event.vk, focused });
    }, nullptr);

    Window::on_key_repeat.subscribe([](Window::KeyEvent event, void *) -> void {
        if (focused)
            focused->on_key_repeat.fire({ KeyEvent::UI_KEY_REPEAT, event.vk, focused });
    }, nullptr);
}

void UiContainer::cleanup()
{
    destroy(root_container);
    Event<MouseEvent>::destroy(static_on_click);
    deferred_callbacks.clear();
}

void UiContainer::render()
{
    if (hide)
        return;

    UiStyle style = UiStyle::get_style<UiContainer>(style_tags);

    Colour colour;
    try { colour = style.get<Colour>("background-colour"); } catch (...) { }

    UiShadow shadow;
    try { shadow = style.get<UiShadow>("shadow"); } catch (...) { }

    float radius;
    try { radius = style.get<float>("corner-radius"); } catch (...) { }

    UiBorder border;
    try { border = style.get<UiBorder>("border"); } catch (...) { }

    if (shadow.size > 0) {
        RenderTargetPair target_pair;

        Vector2i shadow_size = Vector2i(shadow.size, shadow.size);
        Vector2i shadow_blur = Vector2i(shadow.blur, shadow.blur);

        int shadow_res = 4;

        Vector2i target_size = (calculated_size + shadow_size*2 + shadow_blur*2)/shadow_res;

        try {
            target_pair = shadow_textures.at(this);
        } catch (...) {
            target_pair.a = RenderTarget::create({
                .format = Texture::TEX_RGBA,
                .width = target_size.x,
                .height = target_size.y,
                .depth_buffer = false
            });

            target_pair.b = RenderTarget::create({
                .format = Texture::TEX_RGBA,
                .width = target_size.x,
                .height = target_size.y,
                .depth_buffer = false
            });

            shadow_textures[this] = target_pair;
        }

        target_pair.b->clear();
        target_pair.a->clear();

        Material::defaults.ui_shadow_shape->set("pixel_scale", PIXEL_SCALE);
        Material::defaults.ui_shadow_shape->set("pos", shadow_blur/shadow_res);
        Material::defaults.ui_shadow_shape->set("colour", shadow.colour);
        Material::defaults.ui_shadow_shape->set("size", (calculated_size + shadow_size*2)/shadow_res);
        Material::defaults.ui_shadow_shape->set("depth", (float)calculated_depth);
        Material::defaults.ui_shadow_shape->set("max_depth", (float)MAX_DEPTH);
        Material::defaults.ui_shadow_shape->set("window_size", target_size);
        Material::defaults.ui_shadow_shape->bind();

        Mesh::defaults.ui_quad->draw();

        Vector2i blend_size = target_size;
        blend_size.x -= blend_size.x % 2;
        blend_size.y -= blend_size.y % 2;

        Material::defaults.ui_shadow->set("pixel_scale", PIXEL_SCALE);
        Material::defaults.ui_shadow->set("pos", Vector2i(0, 0));
        Material::defaults.ui_shadow->set("size", blend_size);
        Material::defaults.ui_shadow->set("depth", (float)calculated_depth);
        Material::defaults.ui_shadow->set("max_depth", (float)MAX_DEPTH);
        Material::defaults.ui_shadow->set("window_size", target_size);

        glBlendFunc(GL_ONE, GL_ZERO);

        int horizontal = 1;

        for (int i = 0; i < 4; ++i) {
            RenderTarget *dest = horizontal ? target_pair.b : target_pair.a;
            RenderTarget *source = horizontal ? target_pair.a : target_pair.b;

            dest->bind();

            Material::defaults.ui_shadow->set("horizontal", horizontal);
            Material::defaults.ui_shadow->texture = source->texture;
            Material::defaults.ui_shadow->bind();

            Mesh::defaults.ui_quad->draw();

            horizontal = !horizontal;
        }

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        RenderTarget::unbind();

        Material::defaults.ui_container->set("pixel_scale", PIXEL_SCALE);
        Material::defaults.ui_container->set("pos", calculated_position - shadow_blur - shadow_size);
        Material::defaults.ui_container->set("size", target_size*shadow_res);
        Material::defaults.ui_container->set("depth", (float)calculated_depth - 0.75f);
        Material::defaults.ui_container->set("max_depth", (float)MAX_DEPTH);
        Material::defaults.ui_container->set("window_size", Window::true_size);
        Material::defaults.ui_container->set("use_texture", 1);
        Material::defaults.ui_container->texture = target_pair.a->texture;
        Material::defaults.ui_container->bind();

        Mesh::defaults.ui_quad->draw();
    }

    Vector2i draw_size = calculated_size;
    Vector2i draw_position = calculated_position;

    if (border.size > 0) {
        Material::defaults.ui_container->set("pixel_scale", PIXEL_SCALE);
        Material::defaults.ui_container->set("colour", border.colour);
        Material::defaults.ui_container->set("radius", radius);
        Material::defaults.ui_container->set("pos", calculated_position);
        Material::defaults.ui_container->set("size", calculated_size);
        Material::defaults.ui_container->set("depth", (float)calculated_depth - 0.25f);
        Material::defaults.ui_container->set("max_depth", (float)MAX_DEPTH);
        Material::defaults.ui_container->set("window_size", Window::true_size);
        Material::defaults.ui_container->set("use_texture", 0);
        Material::defaults.ui_container->bind();

        Mesh::defaults.ui_quad->draw();

        draw_size.x -= border.size*2;
        draw_size.y -= border.size*2;
        draw_position.x += border.size;
        draw_position.y += border.size;
    }

    Material::defaults.ui_container->set("pixel_scale", PIXEL_SCALE);
    Material::defaults.ui_container->set("colour", colour);
    Material::defaults.ui_container->set("radius", radius);
    Material::defaults.ui_container->set("pos", draw_position);
    Material::defaults.ui_container->set("size", draw_size);
    Material::defaults.ui_container->set("depth", (float)calculated_depth);
    Material::defaults.ui_container->set("max_depth", (float)MAX_DEPTH);
    Material::defaults.ui_container->set("window_size", Window::true_size);
    Material::defaults.ui_container->set("use_texture", (int)(texture != nullptr));
    Material::defaults.ui_container->texture = texture;
    Material::defaults.ui_container->bind();

    Mesh::defaults.ui_quad->draw();

    on_render.fire(this);

    for (auto child : children) {
        child->render();
    }
}

void UiContainer::set_margin(int margin)
{
    left_margin = margin;
    right_margin = margin;
    top_margin = margin;
    bottom_margin = margin;
}

void UiContainer::set_padding(int padding)
{
    left_padding = padding;
    right_padding = padding;
    top_padding = padding;
    bottom_padding = padding;
}

void UiContainer::delete_children()
{
    for (auto child : children) {
        cleanup_recursive(child);
    }

    children = std::vector<UiContainer *>();
}

void UiContainer::set_style_tags(const std::vector<std::string> &tags)
{
    style_tags = tags;
    UiStyle style = UiStyle::get_style<UiContainer>(tags);

    try {
        style.state_values.at(UI_PRESSED);

        if (!click_subscription)
            click_subscription = on_click.subscribe(&handle_click, nullptr);
    } catch (...) {
        if (click_subscription) {
            on_click.unsubscribe(click_subscription);
            click_subscription = nullptr;
        }
    }

    try {
        style.state_values.at(UI_HOVERED);

        if (!hover_subscription) {
            hover_subscription = on_hover.subscribe(&handle_hover, nullptr);
            off_hover_subscription = off_hover.subscribe(&handle_off_hover, nullptr);
        }
    } catch (...) {
        if (hover_subscription) {
            on_hover.unsubscribe(hover_subscription);
            off_hover.unsubscribe(off_hover_subscription);

            hover_subscription = nullptr;
            off_hover_subscription = nullptr;
        }
    }
}

bool UiContainer::is_child_of(UiContainer *container)
{
    UiContainer *current = parent;

    while (current) {
        if (current == container)
            return true;

        current = current->parent;
    }

    return false;
}

void UiContainer::calculate_non_grow_size()
{
    if ((size.x == UI_SIZE_FIT || size.y == UI_SIZE_FIT) && calculate_fit_size_override) {
        calculate_fit_size_override(this);
        return;
    }

    const size_t x_axis = offsetof(Vector2i, x);
    const size_t y_axis = offsetof(Vector2i, y);

    size_t axis;
    size_t p_axis;

    switch (layout) {
        case UI_DOWN:
            axis = offsetof(Vector2i, y);
            p_axis = offsetof(Vector2i, x);
            break;
        case UI_RIGHT:
            axis = offsetof(Vector2i, x);
            p_axis = offsetof(Vector2i, y);
            break;
    }

    for (auto &child : children) {
        child->calculate_non_grow_size();
    }

    if (UI_AXIS(size, axis) >= 0) {
        UI_AXIS(calculated_size, axis) = UI_AXIS(size, axis);
    } else if (UI_AXIS(size, axis) == UI_SIZE_FIT) {
        UI_AXIS(calculated_size, axis) = 0;

        for (auto &child : children) {
            switch (child->positioning) {
                case UI_RELATIVE_STATIC:
                    throw std::runtime_error("Not handled.");
                case UI_ABSOLUTE:
                case UI_RELATIVE:
                    continue;
                default:
                break;
            }

            UI_AXIS(calculated_size, axis) += UI_AXIS(child->calculated_size, axis);

            if (axis == x_axis)
                UI_AXIS(calculated_size, axis) += child->right_margin + child->left_margin;
            else
                UI_AXIS(calculated_size, axis) += child->top_margin + child->bottom_margin;
        }
    }

    if (UI_AXIS(size, p_axis) >= 0) {
        UI_AXIS(calculated_size, p_axis) = UI_AXIS(size, p_axis);
    } else if (UI_AXIS(size, p_axis) == UI_SIZE_FIT) {
        UI_AXIS(calculated_size, p_axis) = 0;

        for (auto &child : children) {
            switch (child->positioning) {
                case UI_RELATIVE_STATIC:
                    throw std::runtime_error("Not handled.");
                case UI_ABSOLUTE:
                case UI_RELATIVE:
                    continue;
                default:
                break;
            }

            int margins;

            if (p_axis == x_axis)
                margins = child->right_margin + child->left_margin;
            else
                margins = child->top_margin + child->bottom_margin;

            UI_AXIS(calculated_size, p_axis) = std::max(
                UI_AXIS(calculated_size, p_axis),
                UI_AXIS(child->calculated_size, p_axis) + margins
            );
        }
    }

    UI_AXIS(calculated_size, x_axis) += left_padding + right_padding;
    UI_AXIS(calculated_size, y_axis) += top_padding + bottom_padding;
}

void UiContainer::calculate_children_grow_sizes()
{
    const size_t x_axis = offsetof(Vector2i, x);
    const size_t y_axis = offsetof(Vector2i, y);

    size_t axis;
    size_t p_axis;

    switch (layout) {
        case UI_DOWN:
            axis = offsetof(Vector2i, y);
            p_axis = offsetof(Vector2i, x);
            break;
        case UI_RIGHT:
            axis = offsetof(Vector2i, x);
            p_axis = offsetof(Vector2i, y);
            break;
    }

    int axis_grow_sizes = 0;
    int available_pixels = UI_AXIS(calculated_size, axis);

    if (axis == x_axis)
        available_pixels -= left_padding + right_padding;
    else
        available_pixels -= top_padding + bottom_padding;

    for (auto &child : children) {
        if (child->positioning != UI_STATIC) {
            if (child->size.x == UI_SIZE_GROW)
                child->calculated_size.x = calculated_size.x;
            if (child->size.y == UI_SIZE_GROW)
                child->calculated_size.y = calculated_size.y;

            continue;
        }

        if (UI_AXIS(child->size, axis) == UI_SIZE_GROW)
            ++axis_grow_sizes;

        available_pixels -= UI_AXIS(child->calculated_size, axis);

        if (axis == x_axis)
            available_pixels -= child->left_margin + child->right_margin;
        else
            available_pixels -= child->top_margin + child->bottom_margin;

        if (UI_AXIS(child->size, p_axis) == UI_SIZE_GROW) {
            UI_AXIS(child->calculated_size, p_axis) = UI_AXIS(calculated_size, p_axis);

            if (p_axis == x_axis)
                UI_AXIS(child->calculated_size, p_axis) -= child->right_margin + child->left_margin;
            else
                UI_AXIS(child->calculated_size, p_axis) -= child->top_margin + child->bottom_margin;
        }
    }

    if (axis_grow_sizes) {
        int per_grow = available_pixels/axis_grow_sizes;

        for (auto &child : children) {
            if (child->positioning != UI_STATIC)
                continue;

            if (UI_AXIS(child->size, axis) == UI_SIZE_GROW)
                UI_AXIS(child->calculated_size, axis) += per_grow;
        }
    }

    for (auto &child : children) {
        child->calculate_children_grow_sizes();
    }
}

void UiContainer::position_children()
{
    const size_t x_axis = offsetof(Vector2i, x);
    const size_t y_axis = offsetof(Vector2i, y);

    size_t axis;
    size_t p_axis;

    switch (layout) {
        case UI_DOWN:
            axis = offsetof(Vector2i, y);
            p_axis = offsetof(Vector2i, x);
            break;
        case UI_RIGHT:
            axis = offsetof(Vector2i, x);
            p_axis = offsetof(Vector2i, y);
            break;
    }

    Vector2i available_pixels = calculated_size;
    available_pixels.x -= left_padding + right_padding;
    available_pixels.y -= top_padding + bottom_padding;

    Vector2i origin = calculated_position;
    origin.x += left_padding;
    origin.y += top_padding;

    int children_size = 0;

    for (auto &child : children) {
        if (child->positioning != UI_STATIC)
            continue;

        children_size += UI_AXIS(child->calculated_size, axis);

        if (axis == x_axis)
            children_size += child->left_margin + child->right_margin;
        else
            children_size += child->top_margin + child->bottom_margin;
    }

    int base_position;
    int base_position_center = UI_AXIS(origin, axis) + UI_AXIS(available_pixels, axis)/2 - children_size/2;

    switch (alignment) {
        case UI_TOP_LEFT:
            base_position = UI_AXIS(origin, axis);
            break;
        case UI_CENTER:
            base_position = base_position_center;
            break;
        case UI_CENTER_LEFT:
            if (axis == x_axis)
                base_position = UI_AXIS(origin, x_axis);
            else
                base_position = base_position_center;
            break;
    }

    int cursor = 0;

    for (auto child : children) {
        switch (child->positioning) {
            case UI_ABSOLUTE:
                child->calculated_position = child->position;
                child->calculated_depth = child->depth;
                child->on_update.fire(child);
                child->position_children();
                if (!non_static_containers.contains(child))
                    non_static_containers.insert(child);
                continue;
            case UI_RELATIVE:
                child->calculated_position = calculated_position + child->position;
                child->calculated_depth = calculated_depth + child->depth;
                child->on_update.fire(child);
                child->position_children();
                if (!non_static_containers.contains(child))
                    non_static_containers.insert(child);
                continue;
            break;
            case UI_RELATIVE_STATIC:
                throw std::runtime_error("Not implemented.");
            break;
            default:
                if (non_static_containers.contains(child))
                    non_static_containers.erase(child);
            break;
        }

        if (axis == x_axis)
            cursor += child->left_margin;
        else
            cursor += child->top_margin;

        UI_AXIS(child->calculated_position, axis) = base_position + cursor;
        child->calculated_depth = calculated_depth + 1;

        int p_axis_center = UI_AXIS(origin, p_axis) + UI_AXIS(available_pixels, p_axis)/2 - UI_AXIS(child->calculated_size, p_axis)/2;

        int margin;

        if (p_axis == x_axis)
            margin = child->left_margin;
        else
            margin = child->top_margin;

        switch (alignment) {
            case UI_TOP_LEFT:
                UI_AXIS(child->calculated_position, p_axis) = UI_AXIS(origin, p_axis) + margin;
                break;
            case UI_CENTER:
                UI_AXIS(child->calculated_position, p_axis) = p_axis_center;
                break;
            case UI_CENTER_LEFT:
                if (axis == x_axis)
                    child->calculated_position.y = p_axis_center;
                else
                    child->calculated_position.y = UI_AXIS(origin, x_axis) + margin;
                break;
        }

        cursor += UI_AXIS(child->calculated_size, axis);

        if (axis == x_axis)
            cursor += child->right_margin;
        else
            cursor += child->bottom_margin;

        child->on_update.fire(child);
        child->position_children();
    }
}

void UiContainer::propagate_mouse_event(Window::MouseEvent event)
{
    UiContainer *deepest_non_static = nullptr;

    for (auto non_static_container : non_static_containers) {
        UiContainer *hit = non_static_container->test_mouse_recursive(event);

        if (hit) {
            if (!deepest_non_static)
                deepest_non_static = hit;
            else
                deepest_non_static = hit->calculated_depth > deepest_non_static->calculated_depth ? hit : deepest_non_static;
        }
    }

    UiContainer *deepest_layout = root_container->test_mouse_recursive(event);
    UiContainer *deepest = nullptr;

    if (deepest_non_static) {
        if (!deepest_layout || deepest_non_static->calculated_depth > deepest_layout->calculated_depth)
            deepest = deepest_non_static;
    } else if (deepest_layout) {
        deepest = deepest_layout;
    }

    switch (event.type) {
        case Window::MouseEvent::MOUSE_CLICK:
            static_on_click.fire({ MouseEvent::UI_CLICK, event.position, event.button, deepest });

            if (deepest != focused) {
                if (focused)
                    focused->off_focus.fire(focused);

                if (deepest)
                    deepest->on_focus.fire(deepest);

                focused = deepest;
            }

            if (deepest) {
                deepest->on_click.fire({ MouseEvent::UI_CLICK, event.position, event.button, deepest });
                clicked = deepest;
            }
        break;
        case Window::MouseEvent::MOUSE_OFF_CLICK:
            if (clicked) {
                clicked->off_click.fire({ MouseEvent::UI_OFF_CLICK, event.position, event.button, clicked });
                clicked = nullptr;
            }
        case Window::MouseEvent::MOUSE_MOVE:
            if (deepest)
                deepest->on_mouse_move.fire({ MouseEvent::UI_MOUSE_MOVE, event.position, event.button, deepest });

            if (deepest == hovered)
                break;

            if (hovered)
                hovered->off_hover.fire({ MouseEvent::UI_OFF_HOVER, event.position, event.button, hovered });

            if (deepest)
                deepest->on_hover.fire({ MouseEvent::UI_HOVER, event.position, event.button, deepest });

            hovered = deepest;
        break;
    }
}

void UiContainer::handle_click(MouseEvent event, void *)
{

}

void UiContainer::handle_hover(MouseEvent event, void *)
{
    UiStyle style = UiStyle::get_style<UiContainer>(event.container->style_tags);
    UiStyle::Values &values = style.state_values[UI_HOVERED];

    for (auto &value_pair : values) {
        if (value_pair.first == "cursor") {
            if (value_pair.second.type != UiStyle::Value::UI_CURSOR)
                continue;

            Window::set_cursor(*(Window::Cursor **)value_pair.second.data);
        }
    }
}

void UiContainer::handle_off_hover(MouseEvent event, void *)
{
    UiStyle style = UiStyle::get_style<UiContainer>(event.container->style_tags);
    UiStyle::Values &values = style.state_values[UI_HOVERED];

    for (auto &value_pair : values) {
        if (value_pair.first == "cursor") {
            if (value_pair.second.type != UiStyle::Value::UI_CURSOR)
                continue;

            Window::set_cursor(nullptr);
        }
    }
}

bool UiContainer::test_mouse(Window::MouseEvent mouse_event)
{
    if (hide)
        return false;

    int mx = mouse_event.position.x;
    int my = mouse_event.position.y;

    int min_cx = calculated_position.x;
    int min_cy = calculated_position.y;
    int max_cx = min_cx + calculated_size.x;
    int max_cy = min_cy + calculated_size.y;

    if (mx > min_cx && mx < max_cx && my > min_cy && my < max_cy)
       return true;

    return false;
}

UiContainer *UiContainer::test_mouse_recursive(Window::MouseEvent mouse_event)
{
    if (!test_mouse(mouse_event))
        return nullptr;

    for (auto child : children) {
        switch (child->positioning) {
            case UI_ABSOLUTE:
            case UI_RELATIVE:
            case UI_RELATIVE_STATIC:
                continue;
            default:
            break;
        }

        UiContainer *child_hit = child->test_mouse_recursive(mouse_event);
        if (child_hit)
            return child_hit;
    }

    switch (mouse_event.type) {
        case Window::MouseEvent::MOUSE_CLICK:
            if (on_click.subscriptions.head || on_focus.subscriptions.head)
                return this;
            break;
        case Window::MouseEvent::MOUSE_OFF_CLICK:
            if (off_click.subscriptions.head)
                return this;
            break;
        case Window::MouseEvent::MOUSE_MOVE:
            if (on_hover.subscriptions.head)
                return this;
            if (on_mouse_move.subscriptions.head)
                return this;
            break;
    }

    return nullptr;
}
