#version 330

out vec4 FragColor;

in vec2 texture_coord;
in vec3 normal;
in vec3 position;

uniform vec3 u_light_color;
uniform vec3 u_light_position;
uniform mat3 u_normal_matrix;
uniform mat4 u_model_matrix;
uniform vec3 u_camera_position;

uniform sampler2D u_texture1;

void main()
{
	// ambiental
	vec3 ambiental = u_light_color*0.3;

	// diffuse 
	vec3 light_vector = normalize(u_light_position - (u_model_matrix*vec4(position, 1.0f)).xyz);
	vec3 normal_vector = normalize(u_normal_matrix * normal);
	vec3 diffuse = u_light_color * clamp(dot(light_vector, normal_vector), 0.0, 1.0);

	// texture color
	vec3 texture_color = texture2D(u_texture1, texture_coord).xyz;

	// phong
	vec3 lambertian = (ambiental + diffuse) * texture_color; 
	FragColor = vec4(lambertian, 1.0f);
}