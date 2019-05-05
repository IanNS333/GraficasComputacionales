#version 330

in vec3 position_in;
in vec3 offset_in;
in vec3 normal_in;
in vec2 texture_coord_up_in;
in vec2 texture_coord_side_in;
in vec2 texture_coord_down_in;
in float face_in;
in vec2 texture_offset_in;

uniform float u_time;
uniform mat4 u_mvp_matrix;
uniform float u_position_offset;
uniform mat4 u_light_mvp_matrix;

out vec3 normal;
out vec3 position;
out vec4 light_pixel_position;
out vec4 pixel_position;
out vec2 texture_coord_up;
out vec2 texture_coord_side;
out vec2 texture_coord_down;
out float face;
out vec2 texture_offset;

void main(){
	mat4 translation = mat4(1);
	translation[3][0] = offset_in.x;
	translation[3][1] = offset_in.y;
	translation[3][2] = offset_in.z;

	vec4 model= translation * vec4(position_in, 1.0);

	vec4 temp = u_mvp_matrix * model;
	light_pixel_position = u_light_mvp_matrix * model;
//	vec4 temp = vec4(position_in, 1.0);

//	gl_Position = temp + vec4(u_position_offset + 1.0, u_position_offset + 1.0, u_position_offset, 0.0);
	gl_Position = temp;
	pixel_position = model;
	normal = normal_in;
	position = position_in;
	texture_coord_up = texture_coord_up_in;
	texture_coord_side = texture_coord_side_in;
	texture_coord_down = texture_coord_down_in;
	face = face_in;
	texture_offset = texture_offset_in;
}