#ifndef OB_COLOR_H
#define OB_COLOR_H

typedef float vec4[4];

struct OB_Color {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
};

float *OB_Color_to_vec4(const struct OB_Color *color, vec4 vec4_color);
struct OB_Color *OB_vec4_to_Color(const vec4 vec4_color, struct OB_Color *color);
struct OB_Color *OB_Color_grayscale(struct OB_Color *color, struct OB_Color *grayscale);

#endif