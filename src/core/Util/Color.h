#ifndef _COLOR_H
#define _COLOR_H

typedef float vec4[4];

typedef struct Color {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
} Color;

float *Color_to_vec4(Color *const color, vec4 vec4_color);
Color *vec4_to_Color(const vec4 vec4_color, Color *color);
Color *Color_grayscale(Color *const color, Color *grayscale);

#endif