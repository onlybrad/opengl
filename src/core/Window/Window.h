#ifndef OB_WINDOW_GLFW_H
#define OB_WINDOW_GLFW_H

#include <GLFW/glfw3.h>
#include "../Window/Scene3D.h"

struct OB_Window;
typedef void(*OB_WindowCallback)(struct OB_Window*);

struct OB_WindowThreadArgs {
    struct OB_Window *window;
    OB_WindowCallback callback;
};

struct OB_Window {
    bool should_close;
    int width;
    int height;
    unsigned updates_per_second;
    float delta_time;
    double current_time;
    unsigned long long frame;
    GLFWwindow *glfw_window;
    struct OB_Scene3D *scene;
    OB_WindowCallback input_callback;
    struct OB_WindowThreadArgs logic_thread_args;
};

void OB_Window_init(struct OB_Window *window, int width, int height, const char* title);
void OB_Window_free(struct OB_Window *window);
void OB_Window_set_vsync(struct OB_Window *window, bool on);
void OB_Window_set_scene3D(struct OB_Window *window, struct OB_Scene3D *scene);
void OB_Window_set_updates_per_second(struct OB_Window *window, const unsigned updates_per_seconds);
void OB_Window_drawing_loop(struct OB_Window *window, void(*drawing_function)(struct OB_Window *));
void OB_Window_logic_loop(struct OB_Window *window, void(*logic_function)(struct OB_Window *));
void OB_Window_set_input_callback(struct OB_Window *window, OB_WindowCallback input_callback);
float OB_Window_time(const struct OB_Window *window);

#endif