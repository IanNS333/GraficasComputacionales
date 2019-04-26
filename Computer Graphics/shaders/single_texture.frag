#version 410

in vec2 texture_coords;

out vec4 fragColor;

uniform sampler2D tex;

void main(){
	fragColor = texture(tex, texture_coords);
}
