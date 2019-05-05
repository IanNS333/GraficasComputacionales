#version 330

in vec3 position_in;
in vec3 offset_in;

uniform mat4 u_light_mvp_matrix;

void main() { 
	vec3 vertex_position = position_in;
	
	mat4 translation = mat4(1);
	translation[3][0] = offset_in.x;
	translation[3][1] = offset_in.y;
	translation[3][2] = offset_in.z;

	gl_Position = u_light_mvp_matrix * translation * vec4(position_in, 1.0);
}