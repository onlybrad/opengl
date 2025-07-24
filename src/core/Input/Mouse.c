#include <assert.h>
#include <GLFW/glfw3.h>
#include "Mouse.h"

struct OB_Mouse {
    struct OB_Window *window;
    OB_MouseCallback cursor_callback;
    OB_MouseCallback scroll_callback;
};

static struct OB_Mouse mouse;

static void OB_GLFW_mouse_cursor_callback(GLFWwindow *glfw_window, double x, double y) {
    (void)glfw_window;
    mouse.cursor_callback(mouse.window, x, y);
}

static void OB_GLFW_mouse_scroll_callback(GLFWwindow *glfw_window, double x, double y) {
    (void)glfw_window;
    mouse.scroll_callback(mouse.window, x, y);
}

void OB_Mouse_init(struct OB_Window *window) {
    assert(window != NULL);

    mouse.window = window;
}

void OB_Mouse_set_cursor_callback(OB_MouseCallback cursor_callback) {
    mouse.cursor_callback = cursor_callback;
    glfwSetCursorPosCallback(mouse.window->glfw_window, OB_GLFW_mouse_cursor_callback);
}

void OB_Mouse_set_scroll_callback(OB_MouseCallback scroll_callback) {
    mouse.scroll_callback = scroll_callback;
    glfwSetScrollCallback(mouse.window->glfw_window, OB_GLFW_mouse_scroll_callback); 
}