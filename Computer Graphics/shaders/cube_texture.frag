#version 330

out vec4 FragColor;

in vec2 texture_coord;

uniform sampler2D u_texture1;
uniform sampler2D u_texture2;

void main()
{
	FragColor = mix(
		texture2D(u_texture1, texture_coord),
		texture2D(u_texture2, texture_coord),
		0.5
	);
}