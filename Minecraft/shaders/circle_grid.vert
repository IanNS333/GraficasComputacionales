#version 330

#define INNER_RADIUS 0.5
#define OUTER_RADIUS 0.7
#define SEGMENT_COUNT 20.0

out vec4 InterpolatedColor;
uniform float u_time;

vec2 getPosition(float id){
  float width = 10.0;
  
  float x = mod(id, width);
  float y = floor(id / width);
  
  float u = x / (width - 1.0);
  float v = y / (width - 1.0);
  
  float xOffset = cos(u_time + y * 0.2) * 0.1;
  float yOffset = cos(u_time + x * 0.3) * 0.2;
  
  float ux = u * 2.0 - 1.0 + xOffset;
  float vy = v * 2.0 - 1.0 + yOffset;
  
  vec2 xy = vec2(ux, vy) * 1.2;
  
  return xy;
}

void main()
{  
  vec2 origin = getPosition(floor(gl_VertexID/ 42.0));
  
  float x = floor(mod(gl_VertexID, 42.0) / 2.0);
  float y = mod(gl_VertexID + 1.0, 2.0);
  
  float angle = x / SEGMENT_COUNT * radians(360.0);
  float radius = INNER_RADIUS + y * OUTER_RADIUS;
  
  float u = radius * cos(angle);
  float v = radius * sin(-angle);
  
  vec2 xy = vec2(u, v) * 0.1;
  xy += origin;

  gl_Position = vec4(xy, 0.0, 1.0);
  gl_PointSize = 20.0;
  InterpolatedColor = vec4(0.0, 0.0, 1.0, 1.0);
}

