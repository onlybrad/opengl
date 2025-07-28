#version 330 core

struct Object {
   float shininess;
   vec3 normal;
   int texture_slot;
   bool is_light;
};

struct Fragment {
   vec2 texture_coordinates;
   vec3 position;
   vec4 color;
};

out vec4 frag_color;
flat in Object object;
in Fragment fragment;

uniform vec3 camera_position;
uniform vec3 light_position;
uniform vec4 light_color;
uniform float ambiant_strength;

uniform sampler2D texture00;
uniform sampler2D texture01;
uniform sampler2D texture02;
uniform sampler2D texture03;
uniform sampler2D texture04;
uniform sampler2D texture05;
uniform sampler2D texture06;
uniform sampler2D texture07;
uniform sampler2D texture08;
uniform sampler2D texture09;
uniform sampler2D texture10;
uniform sampler2D texture11;
uniform sampler2D texture12;
uniform sampler2D texture13;
uniform sampler2D texture14;
uniform sampler2D texture15;
uniform sampler2D texture16;
uniform sampler2D texture17;
uniform sampler2D texture18;
uniform sampler2D texture19;
uniform sampler2D texture20;
uniform sampler2D texture21;
uniform sampler2D texture22;
uniform sampler2D texture23;
uniform sampler2D texture24;
uniform sampler2D texture25;
uniform sampler2D texture26;
uniform sampler2D texture27;
uniform sampler2D texture28;
uniform sampler2D texture29;
uniform sampler2D texture30;
uniform sampler2D texture31;

vec4 get_texture(int index, vec2 texture_coordinates);

void main() {
   //background
   if(!object.is_light && object.texture_slot == 0) {
      frag_color = get_texture(0, fragment.texture_coordinates);
   } else if(!object.is_light) {
      vec3 light_direction = normalize(light_position - fragment.position);
      vec3 camera_direction = normalize(camera_position - fragment.position);
      vec3 reflect_direction = reflect(-light_direction, object.normal);

      vec4 diffuse_texture = get_texture(object.texture_slot, fragment.texture_coordinates);

      vec3 ambient = light_color.rgb * vec3(diffuse_texture) * ambiant_strength;
      vec3 diffuse = light_color.rgb * max(dot(object.normal, light_direction), 0.0) * vec3(diffuse_texture);
      vec3 specular = light_color.rgb * pow(max(dot(camera_direction, reflect_direction), 0.0), object.shininess) * vec3(diffuse_texture);

      frag_color = vec4((ambient + diffuse + specular), 1.0);
   } else {
      frag_color = fragment.color;
   }
}

vec4 get_texture(int index, vec2 texture_coordinates) {
   switch(index) {
      case 0:
         return texture2D(texture00, texture_coordinates);
      case 1:
         return texture2D(texture01, texture_coordinates);
      case 2:
         return texture2D(texture02, texture_coordinates);
      case 3:
         return texture2D(texture03, texture_coordinates);
      case 4:
         return texture2D(texture04, texture_coordinates);
      case 5:
         return texture2D(texture05, texture_coordinates);
      case 6:
         return texture2D(texture06, texture_coordinates);
      case 7:
         return texture2D(texture07, texture_coordinates);
      case 8:
         return texture2D(texture08, texture_coordinates);
      case 9:
         return texture2D(texture09, texture_coordinates);
      case 10:
         return texture2D(texture10, texture_coordinates);
      case 11:
         return texture2D(texture11, texture_coordinates);
      case 12:
         return texture2D(texture12, texture_coordinates);
      case 13:
         return texture2D(texture13, texture_coordinates);
      case 14:
         return texture2D(texture14, texture_coordinates);
      case 15:
         return texture2D(texture15, texture_coordinates);
      case 16:
         return texture2D(texture16, texture_coordinates);
      case 17:
         return texture2D(texture17, texture_coordinates);
      case 18:
         return texture2D(texture18, texture_coordinates);
      case 19:
         return texture2D(texture19, texture_coordinates);
      case 20:
         return texture2D(texture20, texture_coordinates);
      case 21:
         return texture2D(texture21, texture_coordinates);
      case 22:
         return texture2D(texture22, texture_coordinates);
      case 23:
         return texture2D(texture23, texture_coordinates);
      case 24:
         return texture2D(texture24, texture_coordinates);
      case 25:
         return texture2D(texture25, texture_coordinates);
      case 26:
         return texture2D(texture26, texture_coordinates);
      case 27:
         return texture2D(texture27, texture_coordinates);
      case 28:
         return texture2D(texture28, texture_coordinates);
      case 29:
         return texture2D(texture29, texture_coordinates);
      case 30:
         return texture2D(texture30, texture_coordinates);
      case 31:
         return texture2D(texture31, texture_coordinates);
      default:
         vec4 default_texture = {1.0, 1.0, 1.0, 1.0};
         return default_texture;
   }
}