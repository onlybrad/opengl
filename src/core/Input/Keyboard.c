#include "Keyboard.h"
#include "../Util/util.h"

#if defined(_WIN32)
#include <windows.h>
#elif defined(__linux__)
#include <fcntl.h>
#include <linux/kd.h>
#include <linux/keyboard.h>
#else
//Don't have mac to test, nor do I care (who the fuck uses mac for gaming?).
    #error "Implementation needed for set_initial_num_lock_state for OS other than Windows and Linux"
#endif

static bool is_numlock_on(void) {
#if defined(_WIN32)
    return GetKeyState(VK_NUMLOCK) & 1;
#elif defined(__linux__)
    //ChatGPT gave me this, too lazy to check if it actually works LUL
    int fd;
    unsigned int leds;
    fd = open("/dev/console", O_NOCTTY);
    ioctl(fd, KDGKBLED, &leds);
    close(fd);
    return leds & KBD_NUMLOCK;
#endif
}

typedef struct Keyboard {
    bool num_lock;
    Window *window;
    Keyboard_Callback callback;
} Keyboard;

static Keyboard keyboard;

static void internal_keyboard_callback(int key, int action, int mods) {
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(keyboard.window->glfw_window, true);
        return;
    }

    if(key == GLFW_KEY_NUM_LOCK && action == GLFW_PRESS) {
        keyboard.num_lock = mods & GLFW_MOD_NUM_LOCK ? true : false;
    }
}

static void empty_keyboard_callback(Window *const window, int key, int scancode, int action, int mods) {
    (void)window;
    (void)key;
    (void)scancode;
    (void)action;
    (void)mods;
}

static void GLFW_keyboard_callback(GLFWwindow* glfw_window, int key, int scancode, int action, int mods) {
    (void)glfw_window;
    internal_keyboard_callback(key, action, mods);
    keyboard.callback(keyboard.window, key, scancode, action, mods);
}

void Keyboard_init(Window *const window) {
    keyboard.window = window;
    keyboard.num_lock = is_numlock_on();
    keyboard.callback = empty_keyboard_callback;
    glfwSetKeyCallback(window->glfw_window, GLFW_keyboard_callback);
}

void Keyboard_set_callback(Keyboard_Callback callback) {
    keyboard.callback = callback;
}

inline bool Keyboard_is_pressed(const int key) {
    return glfwGetKey(keyboard.window->glfw_window, key) == GLFW_PRESS;
}

inline bool Keyboard_numlock(void) {
    return keyboard.num_lock;
}