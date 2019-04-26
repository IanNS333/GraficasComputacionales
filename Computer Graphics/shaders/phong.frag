#version 410

in vec2 texture_coords;
in vec3 pixel_normal;
in vec3 pixel_position;

out vec4 fragColor;

uniform sampler2D tex;
uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform vec3 cameraPosition;

void main(){
	//blinn-phong
	float ambient = 0.1f;
	vec3 l = normalize(lightPosition - pixel_position);
	vec3 n = normalize(pixel_normal);
	float diffuse = max(0.0f, dot(n, l));
	vec3 c = normalize(cameraPosition - pixel_position);
	vec3 h = normalize(l + c);
	float specular = pow(max(0.0f, dot(h, n)), 32);
	vec4 shading = vec4(lightColor * (ambient + diffuse + specular), 1.0f);
	fragColor = shading * texture(tex, texture_coords);
}

//original phong
//float ambient = 0.1f;
//vec3 l = normalize(lightPosition - pixel_position);
//vec3 n = normalize(pixel_normal);
//float diffuse = max(0.0f, dot(n, l));
//vec3 r = reflect(-l, n);
//vec3 c = normalize(cameraPosition - pixel_position);
//float specular = pow(max(0.0f, dot(r, c)), 16);
//vec4 shading = vec4(lightColor * (ambient + diffuse + specular), 1.0f);
//fragColor = shading * texture(tex, texture_coords);
