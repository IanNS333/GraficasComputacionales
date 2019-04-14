#version 330

in vec3 position_in;
in vec2 texture_coord_in;
in vec3 normal_in;

uniform float u_time;
uniform mat4 u_transformationMatrix;
uniform mat4 u_proyection_matrix;
uniform float u_position_offset;

out vec2 texture_coord;
out vec3 normal;
out vec3 position;

void main(){
	vec4 temp = u_proyection_matrix * ((u_transformationMatrix * vec4(position_in, 1.0)) + vec4(u_position_offset*6, 0.0, - 20, 0.0));
//	vec4 temp = vec4(position_in, 1.0);

//	gl_Position = temp + vec4(u_position_offset + 1.0, u_position_offset + 1.0, u_position_offset, 0.0);
	gl_Position = temp;
	normal = normal_in;
	position = position_in;
	texture_coord = texture_coord_in;
}