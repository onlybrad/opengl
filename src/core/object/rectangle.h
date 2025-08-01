#ifndef OB_RECTANGLE_H
#define OB_RECTANGLE_H

#include <stdbool.h>
#include "object.h"
#include "../util/util.h"

#define OB_RECTANGLE_VERTICES(SIDE, COLOR) {\
    {{-SIDE,-SIDE, 0 }, {0.0f, 0.0f, 1.0f}, COLOR, {0.0f, 0.0f}, {0.0f}, {0.0f}, {0.0f}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}},\
    {{ SIDE,-SIDE, 0 }, {0.0f, 0.0f, 1.0f}, COLOR, {1.0f, 0.0f}, {0.0f}, {0.0f}, {0.0f}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}},\
    {{ SIDE, SIDE, 0 }, {0.0f, 0.0f, 1.0f}, COLOR, {1.0f, 1.0f}, {0.0f}, {0.0f}, {0.0f}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}},\
    {{-SIDE,-SIDE, 0 }, {0.0f, 0.0f, 1.0f}, COLOR, {0.0f, 0.0f}, {0.0f}, {0.0f}, {0.0f}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}},\
    {{ SIDE, SIDE, 0 }, {0.0f, 0.0f, 1.0f}, COLOR, {1.0f, 1.0f}, {0.0f}, {0.0f}, {0.0f}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}},\
    {{-SIDE, SIDE, 0 }, {0.0f, 0.0f, 1.0f}, COLOR, {0.0f, 1.0f}, {0.0f}, {0.0f}, {0.0f}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}}\
}

#define OB_BLACK_RECTANGLE_VERTICES(SIDE) OB_RECTANGLE_VERTICES(SIDE, OB_CURLY_BRACKETS({0.0f, 0.0f, 0.0f, 0.0f}))

bool OB_Rectangle_create(struct OB_Object *object);
bool OB_Rectangle_create_background(struct OB_Object *object);

#endif