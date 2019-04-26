#version 410

in vec2 texture_coords;
in vec3 pixel_normal;
in vec3 pixel_position;

out vec4 fragColor;

uniform sampler2D tex;
uniform vec3 lightColor;
uniform vec3 lightPosition;

void main(){
	float ambient = 0.1f;
	vec3 l = normalize(lightPosition - pixel_position);
	vec3 n = normalize(pixel_normal);
	float diffuse = max(0.0f, dot(n, l));
	vec4 shading = vec4(lightColor * (ambient + diffuse), 1.0f);
	fragColor = shading * texture(tex, texture_coords);
}
