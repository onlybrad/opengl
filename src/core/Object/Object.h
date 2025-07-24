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
    unsigned vertices_count;
} Object;

bool Object_init(Object *object, Vertex *vertices, const unsigned vertices_count);
void Object_init_move(Object *object, Vertex *vertices, const unsigned vertices_count);
void Object_free(Object *object);
void Object_set_texture(Object *object, Texture *texture);
void Object_set_rgba_color(Object *object, Color *color);
void Object_set_vec4_color(Object *object, const vec4 color);
void Object_set_texture_slot(Object *object, const unsigned texture_slot);
void Object_set_shininess(Object *object, const unsigned shininess);
void Object_set_model(Object *object, const float model[16]);
void Object_set_is_light(Object *object, const bool is_light);
unsigned Object_get_texture_slot(Object *object);

#endif