#version 410

in vec3 position;
in vec2 texture_pos;
in vec3 normal;

in vec3 offset;
in float tex_offset;

uniform mat4 vp;

out vec2 texture_coords;
out vec3 pixel_normal;
out vec3 pixel_position;

void main(){
	vec3 pos = position + offset;
	gl_Position = vp * vec4(pos, 1.0);
	texture_coords = texture_pos + tex_offset;
	pixel_normal = normal;
	pixel_position = pos.xyz;
}
