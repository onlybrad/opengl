#ifndef _WINDOW_GLFW_H
#define _WINDOW_GLFW_H

#include <GLFW/glfw3.h>
#include "../Renderer/Scene3D.h"

typedef struct Window Window;
typedef void(*WindowCallback)(Window *const);

typedef struct WindowThreadArgs {
    Window *window;
    WindowCallback callback;
} WindowThreadArgs;

struct Window {
    bool shoudClose;
    int width;
    int height;
    unsigned int updates_per_second;
    float delta_time;
    double current_time;
    long long unsigned int frame;
    GLFWwindow *glfw_window;
    Scene3D *scene;
    WindowCallback input_callback;
    WindowThreadArgs logic_thread_args;
};

void Window_init(Window *const window, int width, int height, const char* const title);
void Window_free(Window *const window);
void Window_set_vsync(Window *const window, const bool on);
void Window_set_scene3D(Window *const window, Scene3D *const scene);
void Window_set_updates_per_second(Window *const window, const unsigned int updates_per_seconds);
void Window_drawing_loop(Window *const window, void(*drawing_function)(Window *const));
void Window_logic_loop(Window *const window, void(*logic_function)(Window *const));
void Window_set_input_callback(Window *const window, WindowCallback input_callback);
float Window_time(void);

#endif