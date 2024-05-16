#ifndef _OBJECT_H
#define _OBJECT_H

#include <stdlib.h>
#include <stdbool.h>
#include <cglm/cglm.h>
#include "../OpenGL/VertexLayout.h"
#include "../OpenGL/Texture.h"
#include "../OpenGL/VertexArrayBuffer.h"

typedef struct Vertex1 {
    float position[3];
    float normal[3];
    float color[4];
    float texture_coordinate[2];
    float texture_slot[1];
    float is_light[1]; 
    float model_1[4];
    float model_2[4];
    float model_3[4];
    float model_4[4];
} Vertex1;

typedef struct Object1 {
    Vertex1 *vertices;
    Texture *texture;
    unsigned int vertices_count;
} Object1;

void Object1_init(Object1 *const object, Vertex1 *const vertices, const unsigned int vertices_count);
void Object1_free(Object1 *const object);
void Object1_set_texture(Object1 *const object, Texture *const texture);
void Object1_set_color(Object1 *const object, const float color[static 4]);
void Object1_set_texture_slot(Object1 *const object, const unsigned int texture_slot);
void Object1_set_model(Object1 *const object, const float model[static 16]);
void Object1_set_is_light(Object1 *const object, const bool is_light);
unsigned int Object1_get_texture_slot(Object1 *const object);
void Object1_create_background(Object1 *const object);
void Object1_create_cube(Object1 *const object);
void Object1_create_light_cube(Object1 *const object);

#endif