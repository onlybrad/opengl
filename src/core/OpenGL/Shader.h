#ifndef OB_SHADER_H
#define OB_SHADER_H

#include <cglm/cglm.h>
#include <stdbool.h>
#include "../Util/String.h"

typedef struct Location {
    int location;
    char *uniform;
} Location;

//Hashmap_str_int
typedef const char *str;
#define HASHMAP_K str
#define HASHMAP_V int
#include "../Template/Hashmap.h"
struct OB_Shader {
    unsigned id;
    struct OB_String vertex_shader_src;
    struct OB_String fragment_shader_src;
    struct Hashmap_str_int location_cache;
};

bool OB_Shader_init(struct OB_Shader *shader, const char *vertex_shader_path, const char *fragment_shader_path);
void OB_Shader_free(struct OB_Shader *shader);
void OB_Shader_use(const struct OB_Shader *shader);
int OB_Shader_get_location(struct OB_Shader *shader, const char *name);
bool OB_Shader_set_bool(struct OB_Shader *shader, const char *name, bool value);
bool OB_Shader_set_int(struct OB_Shader *shader, const char *name, int value);
bool OB_Shader_set_float(struct OB_Shader *shader, const char *name, float value);
bool OB_Shader_set_mat4(struct OB_Shader *shader, const char *name, const float *value);
bool OB_Shader_set_vec3(struct OB_Shader *shader, const char *name, const float *value);
bool OB_Shader_set_vec4(struct OB_Shader *shader, const char *name, const float *value);
void OB_Shader_set_bool_l(struct OB_Shader *shader, int location, bool value);
void OB_Shader_set_int_l(struct OB_Shader *shader, int location, int value);
void OB_Shader_set_float_l(struct OB_Shader *shader, int location, float value);
void OB_Shader_set_mat4_l(struct OB_Shader *shader, int location, const float *value);
void OB_Shader_set_vec3_l(struct OB_Shader *shader, int location, const float *value);
void OB_Shader_set_vec4_l(struct OB_Shader *shader, int location, const float *value);

#endif