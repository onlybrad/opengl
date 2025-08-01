#include "keyboard.h"
#include "../util/util.h"

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

static bool OB_is_numlock_on(void) {
#if defined(_WIN32)
    return GetKeyState(VK_NUMLOCK) & 1;
#elif defined(__linux__)
    //ChatGPT gave me this, too lazy to check if it actually works LUL
    int fd;
    unsigned leds;
    fd = open("/dev/console", O_NOCTTY);
    ioctl(fd, KDGKBLED, &leds);
    close(fd);
    return leds & KBD_NUMLOCK;
#endif
}

static bool num_lock;
static OB_Keyboard_Callback callback;

static void OB_internal_keyboard_callback(int key, int action, int mods) {
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(OB_Window_get()->glfw_window, true);
        return;
    }

    if(key == GLFW_KEY_NUM_LOCK && action == GLFW_PRESS) {
        num_lock = mods & GLFW_MOD_NUM_LOCK ? true : false;
    }
}

static void OB_empty_keyboard_callback(int key, int scancode, int action, int mods) {
    (void)key;
    (void)scancode;
    (void)action;
    (void)mods;
}

static void OB_GLFW_keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    (void)window;
    OB_internal_keyboard_callback(key, action, mods);
    callback(key, scancode, action, mods);
}

void OB_Keyboard_init(void) {
    num_lock = OB_is_numlock_on();
    callback = OB_empty_keyboard_callback;
    glfwSetKeyCallback(OB_Window_get()->glfw_window, OB_GLFW_keyboard_callback);
}

void OB_Keyboard_set_callback(OB_Keyboard_Callback cb) {
    callback = cb;
    glfwSetKeyCallback(OB_Window_get()->glfw_window, OB_GLFW_keyboard_callback);
}

bool OB_Keyboard_is_pressed(int key) {
    return glfwGetKey(OB_Window_get()->glfw_window, key) == GLFW_PRESS;
}

bool OB_Keyboard_numlock(void) {
    return num_lock;
}