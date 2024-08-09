//
// Created by Keegan Beaulieu on 2024-07-08.
//

#ifndef UICONTAINER_H
#define UICONTAINER_H

#include <set>

#include "Window.h"
#include "Colour.h"
#include "Event.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "Vector2i.h"
#include "RenderTarget.h"
#include "VirtualKey.h"

#ifdef __CLIENT__
#define PIXEL_SCALE 4
#else
#define PIXEL_SCALE 1
#endif

enum UiLayout
{
    UI_DOWN,
    UI_RIGHT
};

enum UiAlignment
{
    UI_TOP_LEFT,
    UI_CENTER,
    UI_CENTER_LEFT
};

enum UiPositioning
{
    UI_STATIC,
    UI_ABSOLUTE,
    UI_RELATIVE,
    UI_RELATIVE_STATIC
};

#define UI_SIZE_GROW (-1)
#define UI_SIZE_FIT (-2)

#define UI_AXIS(v, a) ((int &)*((unsigned char *)&v + a))
#define UI_AXIS(v, a) ((int &)*((unsigned char *)&v + a))

class UiContainer
{
public:
    struct MouseEvent
    {
        enum Type
        {
            UI_CLICK,
            UI_OFF_CLICK,
            UI_HOVER,
            UI_OFF_HOVER,
            UI_MOUSE_MOVE
        };

        Type type;
        Vector2i position;
        MouseButton mouse_button;
        UiContainer *container;
    };

    struct KeyEvent
    {
        enum Type
        {
            UI_KEY_DOWN,
            UI_KEY_UP,
            UI_KEY_REPEAT
        };

        Type type;
        VirtualKey key;
        UiContainer *container;
    };

    static const int MAX_DEPTH = 10000;

    std::vector<UiContainer *> children;
    UiContainer *parent;

    UiLayout layout;
    UiAlignment alignment;
    UiPositioning positioning;

    Vector2i position;
    Vector2i size;

    int left_padding;
    int right_padding;
    int bottom_padding;
    int top_padding;

    int left_margin;
    int right_margin;
    int bottom_margin;
    int top_margin;
    int depth;

    Texture *texture;
    std::vector<std::string> style_tags;
    bool hide;

    Vector2i calculated_position;
    Vector2i calculated_size;
    int calculated_depth;

    Event<UiContainer *> on_update;
    Event<UiContainer *> on_render;
    Event<UiContainer *> on_destroy;
    Event<UiContainer *> on_focus;
    Event<UiContainer *> off_focus;
    Event<MouseEvent> on_click;
    Event<MouseEvent> off_click;
    Event<MouseEvent> on_hover;
    Event<MouseEvent> off_hover;
    Event<MouseEvent> on_mouse_move;
    Event<KeyEvent> on_key_down;
    Event<KeyEvent> on_key_up;
    Event<KeyEvent> on_key_repeat;

    static UiContainer *root_container;
    static Event<MouseEvent> static_on_click;

    void update();
    void render();
    void set_margin(int margin);
    void set_padding(int padding);
    void delete_children();
    void set_style_tags(const std::vector<std::string> &tags);
    bool is_child_of(UiContainer *container);

    template <typename T>
    static T *alloc(UiContainer *parent);

    static void destroy(UiContainer *container);
    static void defer(void(*callback)(void *), void *data);
    static void fire_deferred();
    static UiContainer *create(UiContainer *parent);
    static void render_tree();
    static void init();
    static void cleanup();
    static void cleanup_recursive(UiContainer *container);

protected:
    void(*calculate_fit_size_override)(UiContainer *container);

private:
    struct RenderTargetPair
    {
        RenderTarget *a;
        RenderTarget *b;
    };

    struct DeferredCallback
    {
        void(*callback)(void *);
        void *data;
    };

    Event<MouseEvent>::Subscription *click_subscription;
    Event<MouseEvent>::Subscription *hover_subscription;
    Event<MouseEvent>::Subscription *off_hover_subscription;
    Event<MouseEvent>::Subscription *off_click_subscription;

    static List<UiContainer> containers;
    static List<DeferredCallback> deferred_callbacks;
    static std::set<UiContainer *> non_static_containers;
    static std::unordered_map<UiContainer *, RenderTargetPair> shadow_textures;
    static UiContainer *hovered;
    static UiContainer *focused;
    static UiContainer *clicked;

    void calculate_non_grow_size();
    void calculate_children_grow_sizes();
    void position_children();

    bool test_mouse(Window::MouseEvent event);
    UiContainer *test_mouse_recursive(Window::MouseEvent event);

    static void propagate_mouse_event(Window::MouseEvent event);
    static void handle_click(MouseEvent event, void *);
    static void handle_off_click(MouseEvent event, void *);
    static void handle_hover(MouseEvent event, void *);
    static void handle_off_hover(MouseEvent event, void *);
    static void handle_key_down(KeyEvent event, void *);
    static void handle_key_up(KeyEvent event, void *);
};

template <typename T>
T *UiContainer::alloc(UiContainer *parent)
{
    T *container = ((List<T> *)&containers)->alloc();

    container->children = std::vector<UiContainer *>();
    container->style_tags = std::vector<std::string>();
    container->positioning = UI_STATIC;
    container->alignment = UI_TOP_LEFT;
    container->size = Vector2i(UI_SIZE_FIT, UI_SIZE_FIT);

    if (parent) {
        container->parent = parent;
        parent->children.push_back((UiContainer *)container);
    }

    return container;
}

#endif //UICONTAINER_H
