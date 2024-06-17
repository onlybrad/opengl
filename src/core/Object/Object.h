#ifndef _OBJECT_H
#define _OBJECT_H

#include <stdbool.h>
#include <cglm/cglm.h>
#include "../OpenGL/VertexLayout.h"
#include "../OpenGL/Texture.h"
#include "../OpenGL/VertexArrayBuffer.h"
#include "../Util/Color.h"

typedef struct Vertex {
    float position[3];
    float normal[3];
    float color[4];
    float texture_coordinate[2];
    float texture_slot[1];
    float shininess[1];
    float is_light[1];
    float model[16];
} Vertex;

typedef struct Object {
    Vertex *vertices;
    Texture *texture;
    unsigned int vertices_count;
} Object;

void Object_init(Object object[static 1], Vertex vertices[static 1], const unsigned int vertices_count);
void Object_init_move(Object object[static 1], Vertex vertices[static 1], const unsigned int vertices_count);
void Object_free(Object object[static 1]);
void Object_set_texture(Object object[static 1], Texture texture[static 1]);
void Object_set_rgba_color(Object object[static 1], Color color[static 1]);
void Object_set_vec4_color(Object object[static 1], const vec4 color);
void Object_set_texture_slot(Object object[static 1], const unsigned int texture_slot);
void Object_set_shininess(Object object[static 1], const unsigned int shininess);
void Object_set_model(Object object[static 1], const float model[static 16]);
void Object_set_is_light(Object object[static 1], const bool is_light);
unsigned int Object_get_texture_slot(Object object[static 1]);

#endif