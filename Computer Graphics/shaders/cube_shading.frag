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
uniform sampler2D u_texture2;

void main()
{
	// ambiental
	vec3 ambiental = u_light_color*0.1;

	// diffuse 
	vec3 light_vector = normalize(u_light_position - (u_model_matrix*vec4(position, 1.0f)).xyz);
	vec3 normal_vector = normalize(u_normal_matrix * normal);
	vec3 diffuse = u_light_color * clamp(dot(light_vector, normal_vector), 0.0, 1.0);

	// specular
	vec3 reflect_vector = reflect(-light_vector, normal_vector);
	vec3 view_vector = normalize(u_camera_position - position);
	vec3 specular =  u_light_color * pow(clamp(dot(light_vector, normal_vector), 0.0, 1.0), 16.0);

	// texture color
	vec3 texture_color = mix(
		texture2D(u_texture1, texture_coord),
		texture2D(u_texture2, texture_coord),
		0.5
	).xyz;

	vec3 phong = (ambiental + diffuse + specular) * texture_color; 
	
//	float camera_facing_percentage = dot(normal_vector, view_vector);
	
	FragColor = vec4(phong, 1.0);
	

	// phong
//	vec3 phong = (specular) * texture_color; 
}