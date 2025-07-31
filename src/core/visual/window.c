#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "window.h"
#include "../opengl/opengl.h"
#include "../thread/thread.h"

struct OB_Window_CallbackWrapper {
    OB_Window_Callback logic_function;
};

static struct OB_Window window;

static void GLFW_resize_callback(GLFWwindow *_, int width, int height) {
    (void)_;
    OB_OpenGL_set_viewport(width, height);
}

bool OB_Window_init(int width, int height, const char* title) {
    window.width = width;
    window.height = height;
    window.current_time = 0.0f;
    window.delta_time = 0.0f;
    window.frame = 0u;
    window.scene = NULL;
    window.updates_per_second = 30u;
    window.should_close = false;
    window.input_callback = NULL;
    
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window.glfw_window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (window.glfw_window == NULL)  {
        OB_DEBUG_ERROR("Failed to create GLFW window");
        glfwTerminate();
        return false;
    }

    glfwSetInputMode(window.glfw_window, GLFW_LOCK_KEY_MODS, GLFW_TRUE);
    glfwSetInputMode(window.glfw_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetFramebufferSizeCallback(window.glfw_window, GLFW_resize_callback);
    glfwMakeContextCurrent(window.glfw_window);
    
    if(! OB_OpenGL_init((GLADloadproc)glfwGetProcAddress)) {
        OB_Window_free();
        return false;
    }

    return true;
}

void OB_Window_free(void) {
    glfwDestroyWindow(window.glfw_window);
    glfwTerminate();
    memset(&window, 0, sizeof(window));
    window.scene = NULL;
    window.input_callback = NULL;
}

struct OB_Window *OB_Window_get(void) {
    return &window;
}

void OB_Window_set_vsync(bool on) {
    glfwMakeContextCurrent(window.glfw_window);
    glfwSwapInterval((int)on);
}

void OB_Window_set_scene3D(struct OB_Scene3D *scene) {
    window.scene = scene;
    glfwSetCursorPos(window.glfw_window, scene->camera->x, scene->camera->y);
}

void OB_Window_set_updates_per_second(unsigned updates_per_seconds) {
    window.updates_per_second = updates_per_seconds;
}

void OB_Window_drawing_loop(OB_Window_Callback drawing_function) {
    assert(drawing_function != NULL);

    while(! glfwWindowShouldClose(window.glfw_window)) {
        window.frame++;
        const double previous_time = window.current_time;
        window.current_time = glfwGetTime();
        window.delta_time = (float)(window.current_time - previous_time);
        drawing_function(&window);
        glfwSwapBuffers(window.glfw_window);
        glfwPollEvents();
    }

    window.should_close = true;
}

static void *logic_loop(void *arg) {
    assert(arg != NULL);

    struct OB_Window_CallbackWrapper *logic_wrapper = (struct OB_Window_CallbackWrapper*)arg;
    OB_Window_Callback logic_function = logic_wrapper->logic_function;

    const double tick = 1.0/(double)window.updates_per_second; 
    double next_update = glfwGetTime();

    while(! window.should_close) {    
        if(next_update < glfwGetTime()) {
            window.input_callback(&window);
            logic_function(&window);
            next_update += tick;
        }
    }

    return NULL;
}

void OB_Window_logic_loop(void(*logic_function)(struct OB_Window *const)) {
    assert(logic_function != NULL);

    static struct OB_Window_CallbackWrapper logic_wrapper;
    logic_wrapper.logic_function = logic_function;

    struct OB_Thread thread;
    OB_Thread_init(&thread, logic_loop, &logic_wrapper);
    OB_Thread_start(&thread);
}

void OB_Window_set_input_callback(OB_Window_Callback input_callback) {
    assert(input_callback != NULL);

    window.input_callback = input_callback;
}

float OB_Window_time(void) {
    return (float)glfwGetTime();
}
