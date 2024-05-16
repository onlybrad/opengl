#version 330 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec4 a_color;
layout (location = 3) in vec2 a_texture_coordinates;
layout (location = 4) in float a_texture_slot;
layout (location = 5) in float a_is_light;
layout (location = 6) in mat4 a_model;

out vec2 texture_coordinates;
out vec3 frag_position;
out vec4 color;
flat out vec3 normal;
flat out int texture_slot;
flat out float is_light;

uniform mat4 view;
uniform mat4 projection;

void main() {
   //background
   if(a_is_light == 0.0 && a_texture_slot == 0.0) {
      gl_Position = vec4(a_position, 1.0);
   } else {
      frag_position = vec3(a_model * vec4(a_position, 1.0));
      //Do the transpose inverse on the CPU for better performance
      //normal = normalize(mat3(transpose(inverse(a_model))) * a_normal);
      normal = normalize(a_normal);
      color = a_color;
      gl_Position = projection * view * a_model * vec4(a_position, 1.0);
   }

   texture_coordinates = a_texture_coordinates;
   texture_slot = int(a_texture_slot);
   is_light = a_is_light;
}