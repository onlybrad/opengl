#version 330 core

#define SHAPE 0
#define LIGHT 1
#define BACKGROUND 2

struct Object {
   float shininess;
   vec3 normal;
   vec4 color;
   int texture_slot;
   int type;
};

struct Fragment {
   vec2 texture_coordinates;
   vec3 position;
};

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec4 a_color;
layout (location = 3) in vec2 a_texture_coordinates;
layout (location = 4) in float a_texture_slot;
layout (location = 5) in float a_shininess;
layout (location = 6) in float a_type;
layout (location = 7) in mat4 a_model;

flat out Object object;
out Fragment fragment;

uniform mat4 view;
uniform mat4 projection;

void main() {
   object.texture_slot = int(a_texture_slot);
   object.type = int(a_type);
   fragment.texture_coordinates = a_texture_coordinates;

   if(object.type == BACKGROUND) {
      gl_Position = vec4(a_position, 1.0);
   } else {
      //Do the transpose inverse on the CPU for better performance
      //normal = normalize(mat3(transpose(inverse(a_model))) * a_normal);
      object.normal = normalize(a_normal);
      object.shininess = a_shininess;
      object.color = a_color;
      fragment.position = vec3(a_model * vec4(a_position, 1.0));
      gl_Position = projection * view * a_model * vec4(a_position, 1.0);
   }
}