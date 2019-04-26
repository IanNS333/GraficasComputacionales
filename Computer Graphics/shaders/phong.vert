#version 410

in vec3 position;
in vec2 texture_pos;
in vec3 normal;

uniform mat4 model;
uniform mat4 vp;
uniform mat3 norm;

out vec2 texture_coords;
out vec3 pixel_normal;
out vec3 pixel_position;

void main(){
	vec4 pos = vec4(position, 1.0f);
	gl_Position = vp * model * pos;
	texture_coords = texture_pos;
	pixel_normal = norm * normal;
	pixel_position = (model * pos).xyz;
}
