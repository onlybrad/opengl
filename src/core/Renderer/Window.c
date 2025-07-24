#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "Window.h"
#include "../Renderer/OpenGL.h"
#include "../Thread/Thread.h"

static void GLFW_resize_callback(GLFWwindow *_, int width, int height) {
    (void)_;
    OpenGL_set_viewport(width, height);
}

void Window_init(Window *window, const int width, const int height, const char* title) {
    assert(window != NULL);

    window->width = width;
    window->height = height;
    window->current_time = 0.0f;
    window->delta_time = 0.0f;
    window->frame = 0;
    window->scene = NULL;
    window->updates_per_second = 30u;
    window->shoudClose = false;
    window->input_callback = NULL;
    
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window->glfw_window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (window->glfw_window == NULL)  {
        fputs("Failed to create GLFW window", stderr);
        glfwTerminate();
        exit(1);
    }

    glfwSetInputMode(window->glfw_window, GLFW_LOCK_KEY_MODS, GLFW_TRUE);
    glfwSetInputMode(window->glfw_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetFramebufferSizeCallback(window->glfw_window, GLFW_resize_callback);
    glfwMakeContextCurrent(window->glfw_window);
    
    OpenGL_init((GLADloadproc)glfwGetProcAddress, glfwTerminate);
}

void Window_free(Window *window) {
    assert(window != NULL);

    glfwDestroyWindow(window->glfw_window);
    glfwTerminate();
}

void Window_set_vsync(Window *window, const bool on) {
    assert(window != NULL);

    glfwMakeContextCurrent(window->glfw_window);
    glfwSwapInterval((int)on);
}

void Window_set_scene3D(Window *window, Scene3D *scene) {
    assert(window != NULL);
    assert(scene != NULL);

    window->scene = scene;
    glfwSetCursorPos(window->glfw_window, scene->perspective_camera->camera.x, scene->perspective_camera->camera.y);
}

inline void Window_set_updates_per_second(Window *window, const unsigned updates_per_seconds) {
    assert(window != NULL);

    window->updates_per_second = updates_per_seconds;
}

void Window_drawing_loop(Window *window, void(*drawing_function)(Window *const)) {
    assert(window != NULL);
    assert(drawing_function != NULL);

    while(! glfwWindowShouldClose(window->glfw_window)) {
        window->frame++;
        const double previous_time = window->current_time;
        window->current_time = glfwGetTime();
        window->delta_time = (float)(window->current_time - previous_time);
        drawing_function(window);
        glfwSwapBuffers(window->glfw_window);
        glfwPollEvents();
    }

    window->shoudClose = true;
}

static void *logic_loop(void *arg) {
    assert(arg != NULL);

    const WindowThreadArgs *const args = (const WindowThreadArgs*)arg;
    Window *const window = args->window;
    WindowCallback logic_callback = args->callback;

    const double tick = 1.0/(double)window->updates_per_second; 
    double next_update = glfwGetTime();

    while(! window->shoudClose) {    
        if(next_update < glfwGetTime()) {
            window->input_callback(window);
            logic_callback(window);
            next_update += tick;
        }
    }

    return NULL;
}

void Window_logic_loop(Window *window, void(*logic_function)(Window *const)) {
    assert(window != NULL);
    assert(logic_function != NULL);

    window->logic_thread_args.window = window;
    window->logic_thread_args.callback = logic_function;

    Thread thread;
    Thread_init(&thread, logic_loop, &window->logic_thread_args);
    Thread_start(&thread);
}

inline void Window_set_input_callback(Window *window, WindowCallback input_callback) {
    assert(window != NULL);
    assert(input_callback != NULL);

    window->input_callback = input_callback;
}

inline float Window_time(void) {
    return (float)glfwGetTime();
}

