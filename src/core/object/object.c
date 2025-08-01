#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "object.h"
#include "../util/util.h"

bool OB_Object_init(struct OB_Object *object, struct OB_Object_Vertex *vertices, unsigned vertices_count) {
    assert(object != NULL);
    assert(vertices != NULL);

    object->texture = NULL;
    object->vertices_count = vertices_count;

    struct OB_Object_Vertex *vertices_copy = (struct OB_Object_Vertex*)calloc((size_t)vertices_count, sizeof(struct OB_Object_Vertex));
    if(vertices_copy == NULL) {
        return false;
    }

    memcpy(vertices_copy, vertices, sizeof(struct OB_Object_Vertex) * (size_t)vertices_count);
    object->vertices = vertices_copy;

    return true;
}

void OB_Object_init_move(struct OB_Object *object, struct OB_Object_Vertex *vertices, unsigned vertices_count) {
    assert(object != NULL);
    assert(vertices != NULL);

    object->texture = NULL;
    object->vertices_count = vertices_count;
    object->vertices = vertices;
}

void OB_Object_free(struct OB_Object *object) {
    assert(object != NULL);

    free(object->vertices);
    object->vertices = NULL;
    memset(object, 0, sizeof(*object));
}

void OB_Object_set_texture(struct OB_Object *object, struct OB_Texture *texture) {
    assert(object != NULL);

    object->texture = texture;
}

void OB_Object_set_rgba_color(struct OB_Object *object, const struct OB_Color *color) {
    for(unsigned i = 0u; i < object->vertices_count; i++) {
        OB_Color_to_vec4(color, object->vertices[i].color);
    } 
}

void OB_Object_set_vec4_color(struct OB_Object *object, const vec4 color) {
    for(unsigned i = 0u; i < object->vertices_count; i++) {
        memcpy(object->vertices[i].color, color, sizeof(vec4));
    } 
}

void OB_Object_set_texture_slot(struct OB_Object *object, unsigned texture_slot) {
    for(unsigned i = 0u; i < object->vertices_count; i++) {
        object->vertices[i].texture_slot[0] = (float)texture_slot;
    }
}

void OB_Object_set_shininess(struct OB_Object *object, unsigned shininess) {
    for(unsigned i = 0u; i < object->vertices_count; i++) {
        object->vertices[i].shininess[0] = (float)shininess;
    }
}

void OB_Object_set_model(struct OB_Object *object, const float model[16]) {
    for(unsigned i = 0u; i < object->vertices_count; i++) {
        memcpy(&object->vertices[i].model, model, OB_MEMBER_SIZE(struct OB_Object_Vertex, model));
    }  
}

void OB_Object_set_type(struct OB_Object *object, enum OB_Object_Type type) {
    for(unsigned i = 0u; i < object->vertices_count; i++) {
        object->vertices[i].type[0] = (float)type;
    }
}

unsigned OB_Object_get_texture_slot(struct OB_Object *object) {
    return (unsigned)object->vertices[0].texture_slot[0];
}
