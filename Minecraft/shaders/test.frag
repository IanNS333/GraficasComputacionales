#version 330

out vec4 FragColor;

in vec3 normal;
in vec3 position;
in vec4 light_pixel_position;
in vec4 pixel_position;
in vec2 texture_coord_up;
in vec2 texture_coord_side;
in vec2 texture_coord_down;
in float face;
in vec2 texture_offset;
in vec4 color;

uniform vec3 u_light_color;
uniform vec3 u_light_position;
uniform mat3 u_normal_matrix;
uniform vec3 u_camera_position;
uniform vec2 u_poisson_disk[64];

uniform sampler2D u_texture1;
uniform sampler2D u_light_texture;


float random(vec4 seed){
    float dot_product = dot(seed, vec4(12.9898,78.233,45.164,94.673));
    return fract(sin(dot_product) * 43758.5453);
}

vec2 poisson(int index){
	return u_poisson_disk[index];
}

float is_pixel_occluded(vec4 pixel, float bias){

	vec4 proj_coords = ((pixel / pixel.w) * 0.5) + 0.5;
	float current_depth = proj_coords.z;
	float visibility = 1.0;
	float closest_depth = texture2D(u_light_texture, proj_coords.xy).r;

	// poisson-alliasing (bands)
//	int samples = 16;
//	for (int i=0;i<samples;i++){
//		closest_depth = texture2D(u_light_texture, proj_coords.xy + poisson(i)/1000.0).r;
//		if (step(closest_depth, current_depth - bias) < 0.5){
//			visibility-=1.0/samples;
//		}
//	}
//	return visibility;

	// random-poisson-alliasing (dots)
	int samples = 16;

	for (int i=0;i<samples;i++){
		int index = int(64.0*random(vec4(pixel_position.xyz, i)))%64;
		closest_depth = texture2D(u_light_texture, proj_coords.xy + poisson(index)/2100.0).r;
		visibility -= 1.0/samples * (1.0 - step(closest_depth, current_depth - bias));
	}
	return visibility;


	// no-alliasing
//	return step(closest_depth, current_depth - bias);
}

void main()
{

	FragColor = color;
}