#include <assert.h>
#include "cube.h"
#include "../util/util.h"

static struct OB_Object_Vertex OB_BACKGROUND_VERTICES[] = OB_BLACK_RECTANGLE_VERTICES(1.0f);
static struct OB_Object_Vertex OB_CUBE_VERTICES[] = OB_CUBE_VERTICES(0.5f, OB_CURLY_BRACKETS({1.0f, 0.5f, 0.31f, 1.0f}));

bool OB_Cube_create_background(struct OB_Object *object) {
    assert(object != NULL);
    
    if(!OB_Object_init(object, OB_BACKGROUND_VERTICES, (unsigned)OB_ARRAY_LEN(OB_BACKGROUND_VERTICES))) {
        return false;
    }

    mat4 model;
    glm_mat4_identity(model);
    OB_Object_set_model(object, (float*)model);

    return true;
}

bool OB_Cube_create(struct OB_Object *object) {
    assert(object != NULL);
    
    if(!OB_Object_init(object, OB_CUBE_VERTICES, (unsigned)OB_ARRAY_LEN(OB_CUBE_VERTICES))) {
        return false;
    }
    
    mat4 model;
    glm_mat4_identity(model);
    OB_Object_set_model(object, (float*)model);
    OB_Object_set_shininess(object, 32u);

    return true;
}

bool OB_Cube_create_light(struct OB_Object *object) {
    assert(object != NULL);
    
    if(!OB_Cube_create(object)) {
        return false;
    }

    OB_Object_set_type(object, OB_OBJECT_LIGHT);

    return true;
}
