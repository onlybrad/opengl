#ifndef _CUBE_H
#define _CUBE_H

#include "Object.h"

#define CURLY_BRACKETS(...) __VA_ARGS__ 

#define CUBE_VERTICES(SIDE, COLOR) {\
    {.position = {-SIDE,  SIDE,  SIDE}, .normal = {0.0f, 0.0f, 1.0f}, .color = COLOR, .texture_coordinate = {0.0f, 1.0f}},\
    {.position = {-SIDE, -SIDE,  SIDE}, .normal = {0.0f, 0.0f, 1.0f}, .color = COLOR, .texture_coordinate = {0.0f, 0.0f}},\
    {.position = { SIDE, -SIDE,  SIDE}, .normal = {0.0f, 0.0f, 1.0f}, .color = COLOR, .texture_coordinate = {1.0f, 0.0f}},\
    {.position = {-SIDE,  SIDE,  SIDE}, .normal = {0.0f, 0.0f, 1.0f}, .color = COLOR, .texture_coordinate = {0.0f, 1.0f}},\
    {.position = { SIDE, -SIDE,  SIDE}, .normal = {0.0f, 0.0f, 1.0f}, .color = COLOR, .texture_coordinate = {1.0f, 0.0f}},\
    {.position = { SIDE,  SIDE,  SIDE}, .normal = {0.0f, 0.0f, 1.0f}, .color = COLOR, .texture_coordinate = {1.0f, 1.0f}},\
\
    {.position = { SIDE,  SIDE,  SIDE}, .normal = {1.0f, 0.0f, 0.0f}, .color = COLOR, .texture_coordinate = {0.0f, 1.0f}},\
    {.position = { SIDE, -SIDE,  SIDE}, .normal = {1.0f, 0.0f, 0.0f}, .color = COLOR, .texture_coordinate = {0.0f, 0.0f}},\
    {.position = { SIDE, -SIDE, -SIDE}, .normal = {1.0f, 0.0f, 0.0f}, .color = COLOR, .texture_coordinate = {1.0f, 0.0f}},\
    {.position = { SIDE,  SIDE,  SIDE}, .normal = {1.0f, 0.0f, 0.0f}, .color = COLOR, .texture_coordinate = {0.0f, 1.0f}},\
    {.position = { SIDE, -SIDE, -SIDE}, .normal = {1.0f, 0.0f, 0.0f}, .color = COLOR, .texture_coordinate = {1.0f, 0.0f}},\
    {.position = { SIDE,  SIDE, -SIDE}, .normal = {1.0f, 0.0f, 0.0f}, .color = COLOR, .texture_coordinate = {1.0f, 1.0f}},\
\
    {.position = { SIDE,  SIDE, -SIDE}, .normal = {0.0f, 0.0f, -1.0f}, .color = COLOR, .texture_coordinate = {0.0f, 1.0f}},\
    {.position = { SIDE, -SIDE, -SIDE}, .normal = {0.0f, 0.0f, -1.0f}, .color = COLOR, .texture_coordinate = {0.0f, 0.0f}},\
    {.position = {-SIDE, -SIDE, -SIDE}, .normal = {0.0f, 0.0f, -1.0f}, .color = COLOR, .texture_coordinate = {1.0f, 0.0f}},\
    {.position = { SIDE,  SIDE, -SIDE}, .normal = {0.0f, 0.0f, -1.0f}, .color = COLOR, .texture_coordinate = {0.0f, 1.0f}},\
    {.position = {-SIDE, -SIDE, -SIDE}, .normal = {0.0f, 0.0f, -1.0f}, .color = COLOR, .texture_coordinate = {1.0f, 0.0f}},\
    {.position = {-SIDE,  SIDE, -SIDE}, .normal = {0.0f, 0.0f, -1.0f}, .color = COLOR, .texture_coordinate = {1.0f, 1.0f}},\
\
    {.position = {-SIDE,  SIDE, -SIDE}, .normal = {-1.0f, 0.0f, 0.0f}, .color = COLOR, .texture_coordinate = {0.0f, 1.0f}},\
    {.position = {-SIDE, -SIDE, -SIDE}, .normal = {-1.0f, 0.0f, 0.0f}, .color = COLOR, .texture_coordinate = {0.0f, 0.0f}},\
    {.position = {-SIDE, -SIDE,  SIDE}, .normal = {-1.0f, 0.0f, 0.0f}, .color = COLOR, .texture_coordinate = {1.0f, 0.0f}},\
    {.position = {-SIDE,  SIDE, -SIDE}, .normal = {-1.0f, 0.0f, 0.0f}, .color = COLOR, .texture_coordinate = {0.0f, 1.0f}},\
    {.position = {-SIDE, -SIDE,  SIDE}, .normal = {-1.0f, 0.0f, 0.0f}, .color = COLOR, .texture_coordinate = {1.0f, 0.0f}},\
    {.position = {-SIDE,  SIDE,  SIDE}, .normal = {-1.0f, 0.0f, 0.0f}, .color = COLOR, .texture_coordinate = {1.0f, 1.0f}},\
\
    {.position = {-SIDE,  SIDE, -SIDE}, .normal = {0.0f, 1.0f, 0.0f}, .color = COLOR, .texture_coordinate = {0.0f, 1.0f}},\
    {.position = {-SIDE,  SIDE,  SIDE}, .normal = {0.0f, 1.0f, 0.0f}, .color = COLOR, .texture_coordinate = {0.0f, 0.0f}},\
    {.position = { SIDE,  SIDE,  SIDE}, .normal = {0.0f, 1.0f, 0.0f}, .color = COLOR, .texture_coordinate = {1.0f, 0.0f}},\
    {.position = {-SIDE,  SIDE, -SIDE}, .normal = {0.0f, 1.0f, 0.0f}, .color = COLOR, .texture_coordinate = {0.0f, 1.0f}},\
    {.position = { SIDE,  SIDE,  SIDE}, .normal = {0.0f, 1.0f, 0.0f}, .color = COLOR, .texture_coordinate = {1.0f, 0.0f}},\
    {.position = { SIDE,  SIDE, -SIDE}, .normal = {0.0f, 1.0f, 0.0f}, .color = COLOR, .texture_coordinate = {1.0f, 1.0f}},\
\
    {.position = {-SIDE, -SIDE,  SIDE}, .normal = {0.0f, -1.0f, 0.0f}, .color = COLOR, .texture_coordinate = {0.0f, 1.0f}},\
    {.position = {-SIDE, -SIDE, -SIDE}, .normal = {0.0f, -1.0f, 0.0f}, .color = COLOR, .texture_coordinate = {0.0f, 0.0f}},\
    {.position = { SIDE, -SIDE, -SIDE}, .normal = {0.0f, -1.0f, 0.0f}, .color = COLOR, .texture_coordinate = {1.0f, 0.0f}},\
    {.position = {-SIDE, -SIDE,  SIDE}, .normal = {0.0f, -1.0f, 0.0f}, .color = COLOR, .texture_coordinate = {0.0f, 1.0f}},\
    {.position = { SIDE, -SIDE, -SIDE}, .normal = {0.0f, -1.0f, 0.0f}, .color = COLOR, .texture_coordinate = {1.0f, 0.0f}},\
    {.position = { SIDE, -SIDE,  SIDE}, .normal = {0.0f, -1.0f, 0.0f}, .color = COLOR, .texture_coordinate = {1.0f, 1.0f}}\
}

#define BLACK_CUBE_VERTICES(SIDE) CUBE_VERTICES(SIDE, CURLY_BRACKETS({0.0f, 0.0f, 0.0f, 0.0f}))

void Cube_create_background(Object *object);
void Cube_create(Object *object);
void Cube_create_light(Object *object);

#endif