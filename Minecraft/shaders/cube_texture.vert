#version 330

#define DELTA_ALPHA 30.0
#define DELTA_BETA 60.0
#define DELTA_GAMMA 30.0

in vec3 position;
in vec2 texture_coord_in;

uniform float u_time;
uniform mat4 u_transformationMatrix;

out vec2 texture_coord;

void main(){

  gl_Position = u_transformationMatrix * vec4(position, 1.0);
  texture_coord = texture_coord_in;
}