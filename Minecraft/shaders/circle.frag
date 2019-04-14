#version 450

out vec4 FragColor;

uniform vec2 u_resolution;
in vec4 InterpolatedColor;

void main()
{
	if(step(0.25, length(gl_FragCoord.xy/u_resolution - vec2(0.5, 0.5))) == 1.0){
		FragColor = InterpolatedColor;
	}
	else {
		discard;
	}
}