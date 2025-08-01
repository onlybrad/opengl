#ifndef OB_OBJECT_H
#define OB_OBJECT_H

#include <stdbool.h>
#include <cglm/cglm.h>
#include "../opengl/vertexlayout.h"
#include "../opengl/texture.h"
#include "../opengl/vertexarraybuffer.h"
#include "../util/color.h"

enum OB_Object_Type {
    OB_OBJECT_SHAPE = 0,
    OB_OBJECT_LIGHT = 1,
    OB_OBJECT_BACKGROUND = 2 
};

struct OB_Object_Vertex {
    float position[3];
    float normal[3];
    float color[4];
    float texture_coordinate[2];
    float texture_slot[1];
    float shininess[1];
    float type[1];
    float model[16];
};

struct OB_Object {
    struct OB_Object_Vertex *vertices;
    struct OB_Texture *texture;
    unsigned vertices_count;
};

bool OB_Object_init(struct OB_Object *object, struct OB_Object_Vertex *vertices, unsigned vertices_count);
void OB_Object_init_move(struct OB_Object *object, struct OB_Object_Vertex *vertices, unsigned vertices_count);
void OB_Object_free(struct OB_Object *object);
void OB_Object_set_texture(struct OB_Object *object, struct OB_Texture *texture);
void OB_Object_set_rgba_color(struct OB_Object *object, const struct OB_Color *color);
void OB_Object_set_vec4_color(struct OB_Object *object, const vec4 color);
void OB_Object_set_texture_slot(struct OB_Object *object, unsigned texture_slot);
void OB_Object_set_shininess(struct OB_Object *object, unsigned shininess);
void OB_Object_set_model(struct OB_Object *object, const float model[16]);
void OB_Object_set_type(struct OB_Object *object, enum OB_Object_Type type);
enum OB_Object_Type OB_Object_get_type(struct OB_Object *object);
unsigned OB_Object_get_texture_slot(struct OB_Object *object);

#endif