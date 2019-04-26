#version 410

in vec3 position;
in vec2 texture_pos;

uniform mat4 mvpMat;

out vec2 texture_coords;

void main(){
	gl_Position = mvpMat * vec4(position, 1.0f);
	texture_coords = texture_pos;
}
