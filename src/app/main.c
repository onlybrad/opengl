#include <stdbool.h>
#include "space.h"
#include "../core/visual/window.h"
#include "../core/input/keyboard.h"
#include "../core/input/mouse.h"

int main(void) {
    OB_Window_init(800, 600, "LearnOpenGL");
    OB_Window_set_vsync(true);
    OB_Window_set_updates_per_second(60);

    const int code = space_init();
    space_free();

    return code;
}