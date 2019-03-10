#version 330

uniform float u_time;

out vec4 InterpolatedColor;

void main()
{
  float width = 10.0;
  
  float x = mod(gl_VertexID, width);
  float y = floor(gl_VertexID / width);
  
  float u = x / (width - 1.0);
  float v = y / (width - 1.0);

  float xOffset = cos(u_time + y * 0.2) * 0.1;
  float yOffset = cos(u_time + x * 0.3) * 0.2;
  
  float ux = u * 2.0 - 1.0 + xOffset;
  float vy = v * 2.0 - 1.0 + yOffset;
  
  vec2 xy = vec2(ux, vy) * 1.2;
  
  gl_Position = vec4(xy, 0.0, 1.0);
  gl_PointSize = 20.0;
  InterpolatedColor = vec4(cos(x), sin(y), 0.0, 1.0);
}
