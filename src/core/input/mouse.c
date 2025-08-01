#include <assert.h>
#include <GLFW/glfw3.h>
#include "mouse.h"

static OB_Mouse_Callback cursor_callback;
static OB_Mouse_Callback scroll_callback;

static void OB_Mouse_cursor_callback(GLFWwindow* window, double xpos, double ypos) {
    (void)window;
    cursor_callback(xpos, ypos);
}

static void OB_Mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    (void)window;
    scroll_callback(xoffset, yoffset);
}

void OB_Mouse_init(void) {
    cursor_callback = NULL;
    scroll_callback = NULL;
}

void OB_Mouse_set_cursor_callback(OB_Mouse_Callback cb) {
    cursor_callback = cb;
    glfwSetCursorPosCallback(OB_Window_get()->glfw_window, OB_Mouse_cursor_callback);
}

void OB_Mouse_set_scroll_callback(OB_Mouse_Callback cb) {
    scroll_callback = cb;
    glfwSetScrollCallback(OB_Window_get()->glfw_window, OB_Mouse_scroll_callback); 
}