#include <math.h>
#include "color.h"

float *OB_Color_to_vec4(const struct OB_Color *color, vec4 vec4_color) {
    vec4_color[0] = (float)(color->r)/255.0f;
    vec4_color[1] = (float)(color->g)/255.0f;
    vec4_color[2] = (float)(color->b)/255.0f;
    vec4_color[3] = (float)(color->a)/255.0f;

    return (float *)vec4_color;
}

struct OB_Color *OB_vec4_to_Color(const vec4 vec4_color, struct OB_Color *color) {
    color->r = (unsigned char)(vec4_color[0] * 255.0f);
    color->g = (unsigned char)(vec4_color[1] * 255.0f);
    color->b = (unsigned char)(vec4_color[2] * 255.0f);
    color->a = (unsigned char)(vec4_color[3] * 255.0f);

    return color;
}

struct OB_Color *OB_Color_grayscale(struct OB_Color *color, struct OB_Color *grayscale) {
    float gray = roundf(
        0.3f  * (float)color->r + 
        0.59f * (float)color->g + 
        0.11f * (float)color->b
    );
    
    grayscale->r =
    grayscale->g =
    grayscale->b = (unsigned char)gray;
    grayscale->a = color->a;

    return grayscale;
}