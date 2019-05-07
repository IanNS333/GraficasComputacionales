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
		closest_depth = texture2D(u_light_texture, proj_coords.xy + poisson(index)/5000.0).r;
		visibility -= 1.0/samples * (1.0 - step(closest_depth, current_depth - bias));
	}
	return visibility;


	// no-alliasing
//	return step(closest_depth, current_depth - bias);
}

float is_between(float left, float x, float right){
	return step(left, x) * step(x, right);
}

void main()
{
	vec2 resolution = vec2(800.0, 800.0);
	if(step(0.005, length(gl_FragCoord.xy/resolution - vec2(0.5, 0.5))) == 0.0){
		FragColor = vec4(1.0, 0.0, 0.0, 1.0);
		return;
	}
	//render distance of 150
//	if(distance(u_camera_position.xz, (pixel_position/pixel_position.w).xz) > 150.0){
//		discard;
//		return;
//	}

	// ambiental
	vec4 ambiental = vec4(u_light_color*0.3, 1.0);

	// diffuse 
	vec3 light_vector = normalize(u_light_position - position);
	vec3 normal_vector = normalize(u_normal_matrix * normal);
	float cosTheta = dot(light_vector, normal_vector);
	vec4 diffuse = vec4(u_light_color * clamp(cosTheta, 0.0, 1.0), 1.0);

	// texture color
	vec4 texture_color = vec4(0.0);

	float is_up = is_between(-0.5,face, 0.5);
	float is_side = is_between(0.5,face, 1.5);
	float is_down = is_between(1.5, face, 2.5);

	texture_color += is_up * texture2D(u_texture1, texture_coord_up + texture_offset).rgba;
	texture_color += is_side * texture2D(u_texture1, texture_coord_side + texture_offset).rgba;
	texture_color += is_down * texture2D(u_texture1, texture_coord_down + texture_offset).rgba;

	// shadow
	float bias = 0.001*tan(acos(cosTheta));
	bias = clamp(bias, 0.0, 0.001);
	float shadow = is_pixel_occluded(light_pixel_position, 0.0001);
	
	// lambertian
	vec4 lambertian = (ambiental + (1.0 - shadow) * diffuse) * texture_color; 


	FragColor = lambertian;
}