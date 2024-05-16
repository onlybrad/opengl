#include <string.h>
#include "Object1.h"
#include "../util.h"
#include "../OpenGL/VertexLayout.h"

#define CURLY_BRACKETS(...) __VA_ARGS__ 

#define CUBE_VERTICES(SIDE, COLOR) {\
    {.position = {-SIDE,  SIDE,  SIDE}, .normal = {0.0f, 0.0f, 1.0f}, .color = COLOR, .texture_coordinate = {0.0f, 1.0f}},\
    {.position = {-SIDE, -SIDE,  SIDE}, .normal = {0.0f, 0.0f, 1.0f}, .color = COLOR, .texture_coordinate = {0.0f, 0.0f}},\
    {.position = { SIDE, -SIDE,  SIDE}, .normal = {0.0f, 0.0f, 1.0f}, .color = COLOR, .texture_coordinate = {1.0f, 0.0f}},\
    {.position = {-SIDE,  SIDE,  SIDE}, .normal = {0.0f, 0.0f, 1.0f}, .color = COLOR, .texture_coordinate = {0.0f, 1.0f}},\
    {.position = { SIDE, -SIDE,  SIDE}, .normal = {0.0f, 0.0f, 1.0f}, .color = COLOR, .texture_coordinate = {1.0f, 0.0f}},\
    {.position = { SIDE,  SIDE,  SIDE}, .normal = {0.0f, 0.0f, 1.0f}, .color = COLOR, .texture_coordinate = {1.0f, 1.0f}},\
\
    {.position = { SIDE,  SIDE,  SIDE}, .normal = {1.0f, 0.0f, 0.0f}, .color = COLOR, .texture_coordinate = {0.0f, 1.0f}},\
    {.position = { SIDE, -SIDE,  SIDE}, .normal = {1.0f, 0.0f, 0.0f}, .color = COLOR, .texture_coordinate = {0.0f, 0.0f}},\
    {.position = { SIDE, -SIDE, -SIDE}, .normal = {1.0f, 0.0f, 0.0f}, .color = COLOR, .texture_coordinate = {1.0f, 0.0f}},\
    {.position = { SIDE,  SIDE,  SIDE}, .normal = {1.0f, 0.0f, 0.0f}, .color = COLOR, .texture_coordinate = {0.0f, 1.0f}},\
    {.position = { SIDE, -SIDE, -SIDE}, .normal = {1.0f, 0.0f, 0.0f}, .color = COLOR, .texture_coordinate = {1.0f, 0.0f}},\
    {.position = { SIDE,  SIDE, -SIDE}, .normal = {1.0f, 0.0f, 0.0f}, .color = COLOR, .texture_coordinate = {1.0f, 1.0f}},\
\
    {.position = { SIDE,  SIDE, -SIDE}, .normal = {0.0f, 0.0f, -1.0f}, .color = COLOR, .texture_coordinate = {0.0f, 1.0f}},\
    {.position = { SIDE, -SIDE, -SIDE}, .normal = {0.0f, 0.0f, -1.0f}, .color = COLOR, .texture_coordinate = {0.0f, 0.0f}},\
    {.position = {-SIDE, -SIDE, -SIDE}, .normal = {0.0f, 0.0f, -1.0f}, .color = COLOR, .texture_coordinate = {1.0f, 0.0f}},\
    {.position = { SIDE,  SIDE, -SIDE}, .normal = {0.0f, 0.0f, -1.0f}, .color = COLOR, .texture_coordinate = {0.0f, 1.0f}},\
    {.position = {-SIDE, -SIDE, -SIDE}, .normal = {0.0f, 0.0f, -1.0f}, .color = COLOR, .texture_coordinate = {1.0f, 0.0f}},\
    {.position = {-SIDE,  SIDE, -SIDE}, .normal = {0.0f, 0.0f, -1.0f}, .color = COLOR, .texture_coordinate = {1.0f, 1.0f}},\
\
    {.position = {-SIDE,  SIDE, -SIDE}, .normal = {-1.0f, 0.0f, 0.0f}, .color = COLOR, .texture_coordinate = {0.0f, 1.0f}},\
    {.position = {-SIDE, -SIDE, -SIDE}, .normal = {-1.0f, 0.0f, 0.0f}, .color = COLOR, .texture_coordinate = {0.0f, 0.0f}},\
    {.position = {-SIDE, -SIDE,  SIDE}, .normal = {-1.0f, 0.0f, 0.0f}, .color = COLOR, .texture_coordinate = {1.0f, 0.0f}},\
    {.position = {-SIDE,  SIDE, -SIDE}, .normal = {-1.0f, 0.0f, 0.0f}, .color = COLOR, .texture_coordinate = {0.0f, 1.0f}},\
    {.position = {-SIDE, -SIDE,  SIDE}, .normal = {-1.0f, 0.0f, 0.0f}, .color = COLOR, .texture_coordinate = {1.0f, 0.0f}},\
    {.position = {-SIDE,  SIDE,  SIDE}, .normal = {-1.0f, 0.0f, 0.0f}, .color = COLOR, .texture_coordinate = {1.0f, 1.0f}},\
\
    {.position = {-SIDE,  SIDE, -SIDE}, .normal = {0.0f, 1.0f, 0.0f}, .color = COLOR, .texture_coordinate = {0.0f, 1.0f}},\
    {.position = {-SIDE,  SIDE,  SIDE}, .normal = {0.0f, 1.0f, 0.0f}, .color = COLOR, .texture_coordinate = {0.0f, 0.0f}},\
    {.position = { SIDE,  SIDE,  SIDE}, .normal = {0.0f, 1.0f, 0.0f}, .color = COLOR, .texture_coordinate = {1.0f, 0.0f}},\
    {.position = {-SIDE,  SIDE, -SIDE}, .normal = {0.0f, 1.0f, 0.0f}, .color = COLOR, .texture_coordinate = {0.0f, 1.0f}},\
    {.position = { SIDE,  SIDE,  SIDE}, .normal = {0.0f, 1.0f, 0.0f}, .color = COLOR, .texture_coordinate = {1.0f, 0.0f}},\
    {.position = { SIDE,  SIDE, -SIDE}, .normal = {0.0f, 1.0f, 0.0f}, .color = COLOR, .texture_coordinate = {1.0f, 1.0f}},\
\
    {.position = {-SIDE, -SIDE,  SIDE}, .normal = {0.0f, -1.0f, 0.0f}, .color = COLOR, .texture_coordinate = {0.0f, 1.0f}},\
    {.position = {-SIDE, -SIDE, -SIDE}, .normal = {0.0f, -1.0f, 0.0f}, .color = COLOR, .texture_coordinate = {0.0f, 0.0f}},\
    {.position = { SIDE, -SIDE, -SIDE}, .normal = {0.0f, -1.0f, 0.0f}, .color = COLOR, .texture_coordinate = {1.0f, 0.0f}},\
    {.position = {-SIDE, -SIDE,  SIDE}, .normal = {0.0f, -1.0f, 0.0f}, .color = COLOR, .texture_coordinate = {0.0f, 1.0f}},\
    {.position = { SIDE, -SIDE, -SIDE}, .normal = {0.0f, -1.0f, 0.0f}, .color = COLOR, .texture_coordinate = {1.0f, 0.0f}},\
    {.position = { SIDE, -SIDE,  SIDE}, .normal = {0.0f, -1.0f, 0.0f}, .color = COLOR, .texture_coordinate = {1.0f, 1.0f}}\
}

#define BLACK_CUBE_VERTICES(SIDE) CUBE_VERTICES(SIDE, CURLY_BRACKETS({0.0f, 0.0f, 0.0f, 0.0f}))

static Vertex1 background_vertices[] = BLACK_CUBE_VERTICES(1.0f);
static Vertex1 cube_vertices[] = CUBE_VERTICES(0.5f, CURLY_BRACKETS({1.0f, 0.5f, 0.31f, 1.0f}));

void Object1_init(Object1 *const object, Vertex1 *const vertices, const unsigned int vertices_count) {
    object->texture = NULL;
    object->vertices_count = vertices_count;
    object->vertices = malloc((size_t)vertices_count * sizeof(Vertex1));
    memcpy(object->vertices, vertices, (size_t)vertices_count * sizeof(Vertex1));
}

void Object1_free(Object1 *const object) {
    free(object->vertices);
    object->vertices = NULL;
}

inline void Object1_set_texture(Object1 *const object, Texture *const texture) {
    object->texture = texture;
}

void Object1_set_color(Object1 *const object, const float color[static 4]) {
    for(unsigned int i=0; i<object->vertices_count; i++) {
        memcpy(&object->vertices[i].color, color, 4 * sizeof(float));
    } 
}

void Object1_set_texture_slot(Object1 *const object, const unsigned int texture_slot) {
    for(unsigned int i=0; i<object->vertices_count; i++) {
        object->vertices[i].texture_slot[0] = (float)texture_slot;
    }
}

void Object1_set_model(Object1 *const object, const float model[static 16]) {
    for(unsigned int i=0; i<object->vertices_count; i++) {
        memcpy(&object->vertices[i].model_1, model, 16 * sizeof(float));
    }  
}

void Object1_set_is_light(Object1 *const object, const bool is_light) {
    for(unsigned int i=0; i<object->vertices_count; i++) {
        object->vertices[i].is_light[0] = is_light ? 1.0f : 0.0f;
    }
}

unsigned int Object1_get_texture_slot(Object1 *const object) {
    return (unsigned int)object->vertices[0].texture_slot[0];
}

void Object1_create_background(Object1 *const object) {
    Object1_init(object, background_vertices, (unsigned int)ARRAY_LENGTH(background_vertices));
    mat4 model;
    glm_mat4_identity(model);
    Object1_set_model(object, (float*)model);
}

void Object1_create_cube(Object1 *const object) {
    Object1_init(object, cube_vertices, (unsigned int)ARRAY_LENGTH(cube_vertices));
    mat4 model;
    glm_mat4_identity(model);
    Object1_set_model(object, (float*)model);
}

void Object1_create_light_cube(Object1 *const object) {
    Object1_create_cube(object);
    Object1_set_is_light(object, true);
}
