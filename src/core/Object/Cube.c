#include <string.h>
#include "Cube.h"
#include "../Util/util.h"

static Vertex background_vertices[] = BLACK_CUBE_VERTICES(1.0f);
static Vertex cube_vertices[] = CUBE_VERTICES(0.5f, CURLY_BRACKETS({1.0f, 0.5f, 0.31f, 1.0f}));

void Cube_create_background(Object *const object) {
    Vertex *background_vertices_copy = malloc(sizeof(background_vertices));
    memcpy(background_vertices_copy, background_vertices, sizeof(background_vertices));
    Object_init(object, background_vertices_copy, (unsigned int)ARRAY_LENGTH(background_vertices));

    mat4 model;
    glm_mat4_identity(model);
    Object_set_model(object, (float*)model);
}

void Cube_create(Object *const object) {
    Vertex *cube_vertices_copy = malloc(sizeof(cube_vertices));
    memcpy(cube_vertices_copy, cube_vertices, sizeof(cube_vertices));

    Object_init(object, cube_vertices_copy, (unsigned int)ARRAY_LENGTH(cube_vertices));
    mat4 model;
    glm_mat4_identity(model);
    Object_set_model(object, (float*)model);
    Object_set_shininess(object, 32u);
}

void Cube_create_light(Object *const object) {
    Cube_create(object);
    Object_set_is_light(object, true);
}
