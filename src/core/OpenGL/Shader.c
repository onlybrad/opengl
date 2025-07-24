#include <assert.h>
#include <glad/glad.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Shader.h"
#include "../Util/util.h"

#define K str
#define VALUE int
#define HASHMAP_IMPLEMENTATION
#include "../Template/HashMap.h"

static unsigned int compile_shader(String shader_source, const GLenum shader_type) {
    const unsigned int shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &shader_source.buffer, (int *)&(shader_source.length));
    glCompileShader(shader);

    int success;
    char info_log[512];

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if(!success) {
        const char *shader_type_name;
        switch(shader_type) {
            case GL_VERTEX_SHADER: shader_type_name = "VERTEX_SHADER"; break;
            case GL_FRAGMENT_SHADER: shader_type_name = "FRAGMENT_SHADER"; break;
            default: shader_type_name = "UNKNOWN_SHADER"; break;
        }

        glGetShaderInfoLog(shader, 512, NULL, info_log);
        fprintf(stderr,"ERROR::%s::COMPILATION_FAILED\n%s\n",shader_type_name, info_log);
        return 0u;
    }

    return shader;
}

static unsigned int create_shader(String vertex_shader_src, String fragment_shader_src) {
    const unsigned int vertex_shader = compile_shader(vertex_shader_src, GL_VERTEX_SHADER);
    if(vertex_shader == 0u) {
        return 0u;
    }

    const unsigned int fragment_shader = compile_shader(fragment_shader_src, GL_FRAGMENT_SHADER);
    if(fragment_shader == 0u) {
        return 0u;
    }

    const unsigned int program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if(!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        fprintf(stderr,"ERROR::PROGRAM::LINKING_FAILED\n%s\n",infoLog);
        return 0u;
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return program;
}

bool Shader_init(Shader *shader, const char *vertex_shader_path, const char *fragment_shader_path) {
    assert(shader != NULL);
    assert(vertex_shader_path != NULL);
    assert(fragment_shader_path != NULL);

    const String vertex_shader_src = file_get_contents(vertex_shader_path);
    if(vertex_shader_src.buffer == NULL) {
        return false;
    }

    const String fragment_shader_src = file_get_contents(fragment_shader_path);
    if(fragment_shader_src.buffer == NULL) {
        String_free(vertex_shader_src);
        return false;
    }

    const unsigned int id = create_shader(vertex_shader_src, fragment_shader_src);
    if(id == 0) {
        String_free(vertex_shader_src);
        String_free(fragment_shader_src);
        return false;
    }

    shader->id = id;
    shader->vertex_shader_src = vertex_shader_src;
    shader->fragment_shader_src = fragment_shader_src;
    HashMap_str_int_init(&shader->location_cache, string_hash_function, string_compare);

    return true;
}

void Shader_free(Shader *shader) {
    assert(shader != NULL);

    glDeleteProgram(shader->id);
    String_free(shader->vertex_shader_src);
    String_free(shader->fragment_shader_src);
    HashMap_str_int_free(&shader->location_cache);
}

void Shader_use(const Shader *shader) {
    assert(shader != NULL);

    glUseProgram(shader->id);
}

int Shader_get_location(Shader *shader, const char *name) {
    assert(shader != NULL);
    assert(name != NULL);

    bool success;
    const int result = HashMap_str_int_get(&shader->location_cache, name, &success);

    if(success) {
        return result;
    }

    int location = glGetUniformLocation(shader->id, name);
    if(location < 0) {
        fprintf(stderr, "Could not find \"%s\" uniform in shader\n", name);
        return location;
    }

    HashMap_str_int_insert(&shader->location_cache, name, location);
    
    return location;
}

bool Shader_set_bool(Shader *shader, const char *name, const bool value) {
    assert(shader != NULL);
    assert(name != NULL);

    int location;
    if((location = Shader_get_location(shader, name)) < 0) {
        return false;
    }

    glUniform1i(location, (int)value);
    
    return true;
}

bool Shader_set_int(Shader *shader, const char *name, const int value) {
    assert(shader != NULL);
    assert(name != NULL);

    int location;
    if((location = Shader_get_location(shader, name)) < 0) {
        return false;
    }
    
    glUniform1i(location, value);
    
    return true;
}

bool Shader_set_float(Shader *shader, const char *name, const float value) {
    assert(shader != NULL);
    assert(name != NULL);

    int location;
    if((location = Shader_get_location(shader, name)) < 0) {
        return false;
    }

    glUniform1f(location, value);
    
    return true;
}

bool Shader_set_mat4(Shader *shader, const char *name, const float *value) {
    assert(shader != NULL);
    assert(name != NULL);
    assert(value != NULL);

    int location;
    if((location = Shader_get_location(shader, name)) < 0) {
        return false;
    }

    glUniformMatrix4fv(location, 1, false, value);

    return true;
}

bool Shader_set_vec3(Shader *shader, const char *name, const float *value) {
    assert(shader != NULL);
    assert(name != NULL);
    assert(value != NULL);

    int location;
    if((location = Shader_get_location(shader, name)) < 0) {
        return false;
    }

    glUniform3fv(location, 1, value);

    return true; 
}

bool Shader_set_vec4(Shader *shader, const char *name, const float *value) {
    assert(shader != NULL);
    assert(name != NULL);
    assert(value != NULL);

    int location;
    if((location = Shader_get_location(shader, name)) < 0) {
        return false;
    }

    glUniform4fv(location, 1, value);

    return true; 
}

inline void Shader_set_bool_l(Shader *shader, const int location, const bool value) {
    assert(shader != NULL);

    (void)shader;
    glUniform1i(location, (int)value);
}

inline void Shader_set_int_l(Shader *shader, const int location, const int value) {
    assert(shader != NULL);

    (void)shader;
    glUniform1i(location, value);    
}

inline void Shader_set_float_l(Shader *shader, const int location, const float value) {
    assert(shader != NULL);

    (void)shader;    
    glUniform1f(location, value);
}

inline void Shader_set_mat4_l(Shader *shader, const int location, const float *value) {
    assert(shader != NULL);

    (void)shader;
    glUniformMatrix4fv(location, 1, false, value);
}

inline void Shader_set_vec3_l(Shader *shader, const int location, const float *value) {
    assert(shader != NULL);

    (void)shader;
    glUniform3fv(location, 1, value);
}

inline void Shader_set_vec4_l(Shader *shader, const int location, const float *value) {
    assert(shader != NULL);
    
    (void)shader;
    glUniform4fv(location, 1, value);
}