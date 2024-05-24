#ifndef _OBJECT_H
#define _OBJECT_H

#include <stdlib.h>
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

void Object_init(Object *const object, Vertex *const vertices, const unsigned int vertices_count);
void Object_free(Object *const object);
void Object_set_texture(Object *const object, Texture *const texture);
void Object_set_rgba_color(Object *const object, Color *const color);
void Object_set_vec4_color(Object *const object, const vec4 color);
void Object_set_texture_slot(Object *const object, const unsigned int texture_slot);
void Object_set_shininess(Object *const object, const unsigned int shininess);
void Object_set_model(Object *const object, const float model[static 16]);
void Object_set_is_light(Object *const object, const bool is_light);
unsigned int Object_get_texture_slot(Object *const object);

#endif