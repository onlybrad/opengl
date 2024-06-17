#include <GLFW/glfw3.h>
#include "Mouse.h"

typedef struct Mouse {
    Window *window;
    MouseCursorCallback cursor_callback;
    MouseScrollCallback scroll_callback;
} Mouse;

static Mouse mouse;

static void GLFW_mouse_cursor_callback(GLFWwindow *glfw_window, double x, double y) {
    (void)glfw_window;
    mouse.cursor_callback(mouse.window, x, y);
}

static void GLFW_mouse_scroll_callback(GLFWwindow *glfw_window, double x, double y) {
    (void)glfw_window;
    mouse.scroll_callback(mouse.window, x, y);
}

void Mouse_init(Window window[static 1]) {
    mouse.window = window;
}

void Mouse_set_cursor_callback(MouseScrollCallback cursor_callback) {
    mouse.cursor_callback = cursor_callback;
    glfwSetCursorPosCallback(mouse.window->glfw_window, GLFW_mouse_cursor_callback);
}

void Mouse_set_scroll_callback(MouseScrollCallback scroll_callback) {
    mouse.scroll_callback = scroll_callback;
    glfwSetScrollCallback(mouse.window->glfw_window, GLFW_mouse_scroll_callback); 
}