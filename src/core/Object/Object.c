#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "Object.h"
#include "../Util/util.h"

void Object_init(Object *object, Vertex *vertices, const unsigned int vertices_count) {
    assert(object != NULL);
    assert(vertices != NULL);

    object->texture = NULL;
    object->vertices_count = vertices_count;

    Vertex *vertices_copy = calloc((size_t)vertices_count, sizeof(Vertex));
    memcpy(vertices_copy, vertices, sizeof(Vertex) * (size_t)vertices_count);
    object->vertices = vertices_copy;
}

void Object_init_move(Object *object, Vertex *vertices, const unsigned int vertices_count) {
    assert(object != NULL);
    assert(vertices != NULL);

    object->texture = NULL;
    object->vertices_count = vertices_count;
    object->vertices = vertices;
}

void Object_free(Object *object) {
    assert(object != NULL);

    free(object->vertices);
    object->vertices = NULL;
}

inline void Object_set_texture(Object *object, Texture *texture) {
    assert(object != NULL);

    object->texture = texture;
}

void Object_set_rgba_color(Object *object, Color *color) {
    for(unsigned int i = 0u; i < object->vertices_count; i++) {
        Color_to_vec4(color, object->vertices[i].color);
    } 
}

void Object_set_vec4_color(Object *object, const vec4 color) {
    for(unsigned int i = 0u; i < object->vertices_count; i++) {
        memcpy(object->vertices[i].color, color, sizeof(vec4));
    } 
}

void Object_set_texture_slot(Object *object, const unsigned int texture_slot) {
    for(unsigned int i = 0u; i < object->vertices_count; i++) {
        object->vertices[i].texture_slot[0] = (float)texture_slot;
    }
}

void Object_set_shininess(Object *object, const unsigned int shininess) {
    for(unsigned int i = 0u; i < object->vertices_count; i++) {
        object->vertices[i].shininess[0] = (float)shininess;
    }
}

void Object_set_model(Object *object, const float model[16]) {
    for(unsigned int i = 0u; i < object->vertices_count; i++) {
        memcpy(&object->vertices[i].model, model, MEMBER_SIZE(Vertex, model));
    }  
}

void Object_set_is_light(Object *object, const bool is_light) {
    for(unsigned int i = 0u; i < object->vertices_count; i++) {
        object->vertices[i].is_light[0] = is_light ? 1.0f : 0.0f;
    }
}

inline unsigned int Object_get_texture_slot(Object *object) {
    return (unsigned int)object->vertices[0].texture_slot[0];
}
