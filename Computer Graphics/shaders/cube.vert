#version 330

#define DELTA_ALPHA 30.0
#define DELTA_BETA 60.0
#define DELTA_GAMMA 30.0

in vec3 color;
in vec3 position;

uniform float u_time;
uniform mat4 u_transformationMatrix;

out vec4 InterpolatedColor;

void main(){

  gl_Position = u_transformationMatrix * vec4(position, 1.0);
  InterpolatedColor = vec4(color, 1.0);

}