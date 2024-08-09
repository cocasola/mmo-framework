//
// Created by Keegan Beaulieu on 2024-07-07.
//

#include "Window.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

GLFWwindow *Window::handle = nullptr;
Event<Vector2i> Window::on_resize;
Event<Window::MouseEvent> Window::on_click;
Event<Window::MouseEvent> Window::off_click;
Event<Window::MouseEvent> Window::on_mouse_move;
Event<Window::KeyEvent> Window::on_key_down;
Event<Window::KeyEvent> Window::on_key_up;
Event<Window::KeyEvent> Window::on_key_repeat;
float Window::aspect;
Vector2i Window::size;
Vector2i Window::true_size;
Window::CursorDefaults Window::cursor_defaults;
std::unordered_map<VirtualKey, bool> Window::key_state;
Vector2i Window::mouse_position;

void Window::init(const Params &params)
{
    if (!glfwInit())
        exit(1);

    key_state[VK_1] = 0;
    key_state[VK_2] = 0;
    key_state[VK_3] = 0;
    key_state[VK_4] = 0;
    key_state[VK_5] = 0;
    key_state[VK_6] = 0;
    key_state[VK_7] = 0;
    key_state[VK_8] = 0;
    key_state[VK_9] = 0;
    key_state[VK_0] = 0;
    key_state[VK_Q] = 0;
    key_state[VK_W] = 0;
    key_state[VK_E] = 0;
    key_state[VK_R] = 0;
    key_state[VK_T] = 0;
    key_state[VK_Y] = 0;
    key_state[VK_U] = 0;
    key_state[VK_I] = 0;
    key_state[VK_O] = 0;
    key_state[VK_P] = 0;
    key_state[VK_A] = 0;
    key_state[VK_S] = 0;
    key_state[VK_D] = 0;
    key_state[VK_F] = 0;
    key_state[VK_G] = 0;
    key_state[VK_H] = 0;
    key_state[VK_J] = 0;
    key_state[VK_K] = 0;
    key_state[VK_L] = 0;
    key_state[VK_Z] = 0;
    key_state[VK_X] = 0;
    key_state[VK_C] = 0;
    key_state[VK_V] = 0;
    key_state[VK_B] = 0;
    key_state[VK_N] = 0;
    key_state[VK_M] = 0;
    key_state[VK_SHIFT] = 0;
    key_state[VK_SPACE] = 0;
    key_state[VK_BACKSPACE] = 0;
    key_state[VK_CTRL] = 0;
    key_state[VK_ALT] = 0;
    key_state[VK_SUPER] = 0;
    key_state[VK_LEFT] = 0;
    key_state[VK_RIGHT] = 0;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_POSITION_X, params.x);
    glfwWindowHint(GLFW_POSITION_Y, params.y);

    handle = glfwCreateWindow(params.width, params.height, params.title.c_str(), nullptr, nullptr);
    aspect = (float)params.width/params.height;
    size = Vector2i(params.width, params.height);
    glfwGetFramebufferSize(handle, &true_size.x, &true_size.y);

    cursor_defaults.edit = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
    cursor_defaults.point = glfwCreateStandardCursor(GLFW_POINTING_HAND_CURSOR);

    glfwSetWindowSizeCallback(handle, [](GLFWwindow *window, int width, int height) -> void {
        aspect = (float)width/height;
        size = Vector2i(width, height);
        glfwGetFramebufferSize(handle, &true_size.x, &true_size.y);
        on_resize.fire({ true_size.x, true_size.y });
    });

    glfwSetMouseButtonCallback(handle, [](GLFWwindow *, int button, int action, int mods) -> void {
        if (action != GLFW_PRESS && action != GLFW_RELEASE)
            return;

        double xposf, yposf;
        glfwGetCursorPos(handle, &xposf, &yposf);

        if (action == GLFW_PRESS) {
            on_click.fire({
                MouseEvent::MOUSE_CLICK,
                (MouseButton)button,
                Vector2i(
                    // The number reported is not in true pixels
                    (int)(xposf*((double)true_size.x/size.x)),
                    (int)(yposf*((double)true_size.y/size.y))
                )
            });
        } else {
            off_click.fire({
                MouseEvent::MOUSE_OFF_CLICK,
                (MouseButton)button,
                Vector2i(
                    // The number reported is not in true pixels
                    (int)(xposf*((double)true_size.x/size.x)),
                    (int)(yposf*((double)true_size.y/size.y))
                )
            });
        }
    });

    glfwSetKeyCallback(handle, [](GLFWwindow* window, int key, int scancode, int action, int mods) -> void {
        if (action == GLFW_PRESS) {
            on_key_down.fire({ KeyEvent::Type::KEY_DOWN, (VirtualKey)key });
            key_state[(VirtualKey)key] = 1;
        } else if (action == GLFW_RELEASE) {
            on_key_up.fire({ KeyEvent::Type::KEY_UP, (VirtualKey)key });
            key_state[(VirtualKey)key] = 0;
        } else if (action == GLFW_REPEAT) {
            on_key_repeat.fire({ KeyEvent::Type::KEY_REPEAT, (VirtualKey)key });
        }
    });

    glfwSetCursorPosCallback(handle, [](GLFWwindow* window, double xpos, double ypos) -> void {
        mouse_position = Vector2i(
            // The number reported is not in true pixels
            (int)(xpos*((double)true_size.x/size.x)),
            (int)(ypos*((double)true_size.y/size.y))
        );

        on_mouse_move.fire({
            MouseEvent::MOUSE_MOVE,
            MOUSE_NONE,
            mouse_position
        });
    });

    if (!handle) {
        std::cerr << "Failed to init window." << std::endl;
        exit(1);
    }

    glfwMakeContextCurrent(handle);

    glewExperimental = true;
    GLenum r = glewInit();
    if (r != GLEW_OK) {
        std::cerr << "Failed to init window: " << glewGetErrorString(r) << std::endl;
        exit(1);
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glViewport(0, 0, true_size.x, true_size.y);
}

void Window::cleanup()
{
    glfwDestroyWindow(handle);
    glfwTerminate();
}

bool Window::poll()
{
    glfwPollEvents();

    if (glfwWindowShouldClose(handle))
        return true;

    return false;
}

void Window::swap()
{
    glfwSwapBuffers(handle);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::set_cursor(Cursor *cursor)
{
    glfwSetCursor(handle, cursor);
}
