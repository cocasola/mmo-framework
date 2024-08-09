//
// Created by Keegan Beaulieu on 2024-07-07.
//

#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include <unordered_map>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <VirtualKey.h>

#include "Vector2i.h"
#include "Event.h"

enum MouseButton
{
    MOUSE_NONE = -1,
    MOUSE_RIGHT = GLFW_MOUSE_BUTTON_RIGHT,
    MOUSE_LEFT = GLFW_MOUSE_BUTTON_LEFT,
    MOUSE_MIDDLE = GLFW_MOUSE_BUTTON_MIDDLE
};

class Window
{
public:
    struct Params
    {
        int width = 800;
        int height = 600;
        int x = 300;
        int y = 300;
        std::string title = "OpenGL Application";
    };

    struct MouseEvent
    {
        enum Type
        {
            MOUSE_CLICK,
            MOUSE_MOVE,
            MOUSE_OFF_CLICK
        };

        Type type;
        MouseButton button;
        Vector2i position;
    };

    struct KeyEvent
    {
        enum Type
        {
            KEY_DOWN,
            KEY_UP,
            KEY_REPEAT
        };

        Type type;
        VirtualKey vk;
    };

    typedef GLFWcursor Cursor;

    struct CursorDefaults
    {
        Cursor *edit;
        Cursor *point;
    };

    static Event<Vector2i> on_resize;
    static Event<MouseEvent> on_click;
    static Event<MouseEvent> off_click;
    static Event<MouseEvent> on_mouse_move;
    static Event<KeyEvent> on_key_down;
    static Event<KeyEvent> on_key_repeat;
    static Event<KeyEvent> on_key_up;

    static Vector2i mouse_position;

    static float aspect;
    static Vector2i size;
    static Vector2i true_size;
    static std::unordered_map<VirtualKey, bool> key_state;

    static CursorDefaults cursor_defaults;

    static void init(const Params &params);
    static void cleanup();
    static bool poll();
    static void swap();
    static void set_cursor(Cursor *cursor);

private:
    static GLFWwindow *handle;
};

#endif //WINDOW_H
