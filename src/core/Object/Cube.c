#include <string.h>
#include "Cube.h"
#include "../Util/util.h"

static Vertex background_vertices[] = BLACK_CUBE_VERTICES(1.0f);
static Vertex cube_vertices[] = CUBE_VERTICES(0.5f, CURLY_BRACKETS({1.0f, 0.5f, 0.31f, 1.0f}));

void Cube_create_background(Object *const object) {
    Object_init(object, background_vertices, (unsigned int)ARRAY_LEN(background_vertices));

    mat4 model;
    glm_mat4_identity(model);
    Object_set_model(object, (float*)model);
}

void Cube_create(Object *const object) {
    Object_init(object, cube_vertices, (unsigned int)ARRAY_LEN(cube_vertices));
    
    mat4 model;
    glm_mat4_identity(model);
    Object_set_model(object, (float*)model);
    Object_set_shininess(object, 32u);
}

void Cube_create_light(Object *const object) {
    Cube_create(object);
    Object_set_is_light(object, true);
}
