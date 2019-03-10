#version 330

#define POINTS_PER_HALF_CIRCLE 120.0
#define DELTA_ALPHA 20.0
#define DELTA_GAMMA 20.0

#define ALPHA_ZERO 45.0
#define BETA_ZERO 0.0
#define GAMMA_ZERO -10.0

#define SCALE 1.0

uniform float u_time;

out vec4 InterpolatedColor;

void main() {
  float theta = radians(floor(gl_VertexID/ POINTS_PER_HALF_CIRCLE)*(360.0/POINTS_PER_HALF_CIRCLE) ) ;
  float phi = radians(mod(gl_VertexID, POINTS_PER_HALF_CIRCLE)*(180.0/POINTS_PER_HALF_CIRCLE));
  
  vec3 xyz = vec3(
    sin(phi)*sin(theta),
    cos(phi),
    sin(phi)*cos(theta)
  ) * SCALE;
  
  float alpha = radians(sin(u_time)*DELTA_ALPHA) + ALPHA_ZERO;
  float beta = BETA_ZERO;
  float gamma = radians(cos(u_time)*DELTA_ALPHA) + GAMMA_ZERO;
  
  mat3 rotationX = mat3(
  	vec3(1.0, 0.0, 0.0),
    vec3(0.0, cos(alpha), sin(alpha)),
    vec3(0.0, -sin(alpha), cos(alpha))
  );
  mat3 rotationY = mat3(
  	vec3(cos(beta), 0.0, -sin(beta)),
    vec3(0.0, 1.0, 0.0),
    vec3(sin(beta), 0.0, cos(beta))
  );
  mat3 rotationZ = mat3(
  	vec3(cos(gamma), sin(gamma), 0.0),
    vec3(-sin(gamma), cos(gamma), 0.0),
    vec3(0.0, 0.0, 1.0)
  );
  
  vec3 finalPosition = rotationX*rotationY*rotationZ*xyz;

  gl_Position = vec4(finalPosition, 1.0);
  InterpolatedColor = vec4(0.0, 0.0, 1.0, 1.0);
  gl_PointSize = 1.0;
}