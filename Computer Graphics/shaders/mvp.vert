#version 410

in vec3 position;
in vec3 color;

uniform mat4 mvpMat;

out vec4 vertColor;

void main(){
	gl_Position = mvpMat * vec4(position, 1.0f);
	vertColor = vec4(color, 1.0f);
}
