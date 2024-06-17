#ifndef _SHADER_H
#define _SHADER_H

#include <cglm/cglm.h>
#include <stdbool.h>
#include "../Util/String.h"

typedef struct Location {
    int location;
    char *uniform;
} Location;

typedef const char *str;
#define K str
#define V int
#include "../Template/Hashmap.h"
typedef struct Shader {
    unsigned int id;
    String vertex_shader_src;
    String fragment_shader_src;
    Hashmap_str_int location_cache;
} Shader;

bool Shader_init(Shader shader[static 1], const char vertex_shader_path[static 1], const char fragment_shader_path[static 1]);
void Shader_free(Shader shader[static 1]);
void Shader_use(const Shader shader[static 1]);
int Shader_get_location(Shader shader[static 1], const char name[static 1]);
bool Shader_set_bool(Shader shader[static 1], const char name[static 1], const bool value);
bool Shader_set_int(Shader shader[static 1], const char name[static 1], const int value);
bool Shader_set_float(Shader shader[static 1], const char name[static 1], const float value);
bool Shader_set_mat4(Shader shader[static 1], const char name[static 1], const float *value);
bool Shader_set_vec3(Shader shader[static 1], const char name[static 1], const float *value);
bool Shader_set_vec4(Shader shader[static 1], const char name[static 1], const float *value);
void Shader_set_bool_l(Shader shader[static 1], const int location, const bool value);
void Shader_set_int_l(Shader shader[static 1], const int location, const int value);
void Shader_set_float_l(Shader shader[static 1], const int location, const float value);
void Shader_set_mat4_l(Shader shader[static 1], const int location, const float *value);
void Shader_set_vec3_l(Shader shader[static 1], const int location, const float *value);
void Shader_set_vec4_l(Shader shader[static 1], const int location, const float *value);

#endif