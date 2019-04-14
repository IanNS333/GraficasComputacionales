#version 330

uniform vec2 u_resolution;
uniform vec2 u_mouse;
uniform float u_time;

out vec4 FragColor;

void main() {

    vec2 p = vec2(gl_FragCoord)/u_resolution;
    vec2 q = p - vec2(0.35, 0.7);

    vec3 color = mix(
        vec3(1.0, 0.4, 0.1), 
        vec3(0.298, 0.4275, 1.0),
        sqrt(p.y)
	);

    float r = 
		0.2 + 
		0.1 * cos(atan(q.y, q.x) * 10. + 
		20.0 * (0.75 + 0.25 * cos(u_time)) * q.x); 

    color *= smoothstep(r, r + 0.005, length(q));

    r = 0.015;
    r += 0.002 * cos(30. * q.y);
    r += exp(-100.*p.y);
    color *= 
        1. - 
        (1. - smoothstep(r, r + 0.002, 
            abs(q.x - 0.25 * sin(2.0 * q.y))))
        * (1. - smoothstep(0.0, 0.01, q.y));

    FragColor = vec4(color, 1.0);
}