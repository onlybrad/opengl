#ifndef _COLOR_H
#define _COLOR_H

typedef float vec4[4];

typedef struct Color {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
} Color;

#define COLOR_TO_VEC4(COLOR) Color_to_vec4((COLOR), (vec4){0})
#define VEC4_TO_COLOR(VEC4_COLOR) vec4_to_Color((VEC4_COLOR), &(Color){0})
#define COLOR_GRAYSCALE(COLOR) Color_grayscale((COLOR), &(Color){0})

float *Color_to_vec4(Color color[static 1], vec4 vec4_color);
Color *vec4_to_Color(const vec4 vec4_color, Color color[static 1]);
Color *Color_grayscale(Color color[static 1], Color grayscale[static 1]);

#endif