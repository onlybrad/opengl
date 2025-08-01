#include <assert.h>
#include <stdlib.h>
#include "rectangle.h"

static struct OB_Object_Vertex OB_RECTANGLE_VERTICES[] = OB_BLACK_RECTANGLE_VERTICES(1.0f);

bool OB_Rectangle_create(struct OB_Object *object) {
    assert(object != NULL);
    
    if(!OB_Object_init(object, OB_RECTANGLE_VERTICES, (unsigned)OB_ARRAY_LEN(OB_RECTANGLE_VERTICES))) {
        return false;
    }

    mat4 model;
    glm_mat4_identity(model);
    OB_Object_set_model(object, (float*)model);

    return true;
}

bool OB_Rectangle_create_background(struct OB_Object *object) {
    assert(object != NULL);
    
    if(!OB_Rectangle_create(object)) {
        return false;
    }

    OB_Object_set_type(object, OB_OBJECT_BACKGROUND);

    return true;
}
