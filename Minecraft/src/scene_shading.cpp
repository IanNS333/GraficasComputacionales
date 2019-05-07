#include "scene_shading.h"

#include <GL/freeglut.h>
#include <algorithm>
#include <iostream>
#include <forward_list>
#include <atomic>
#include <cmath>

#include "cgmath.h"
#include "mat3.h"
#include "mat4.h"
#include "perlin_noise.h"
#include "time_utils.h"
#include "vec2.h"
#include "vec3.h"
#include "vec3d.h"
#include "vec4.h"
#include "utils.h"

using cgmath::lookAt;
using cgmath::mat3;
using cgmath::mat4;
using cgmath::radians;
using cgmath::rotate_around;
using cgmath::rotation_matrix;
using cgmath::translation_matrix;
using cgmath::vec2;
using cgmath::vec3;
using cgmath::vec3d;
using cgmath::vec4;
using cgmath::AreEqual;
using utils::randf;
using std::max;
using std::min;
using std::atomic;
using std::truncf;

float sign(float val) {
	return val < 0.0f ? -1.0f : val > 0.0f ? 1.0f : 0.0f ;
}

vec3 sign(const vec3& v) {
	return vec3(sign(v.x), sign(v.y), sign(v.z));
}

float abs_ceil(float x)
{
	return x < 0 ? floor(x) : ceil(x);
}

scene_shading::~scene_shading() {
    glDeleteProgram(shader_program);
    glDeleteProgram(light_shader_program);
    glDeleteTextures(1, &textureId);
}

void scene_shading::compile_shaders() {
    shader_program = generateShaderProgram(
        {
            {"shaders/shadow.vert", GL_VERTEX_SHADER},
            {"shaders/shadow.frag", GL_FRAGMENT_SHADER}
        },
        {
			"position_in", 
			"offset_in", 
			"normal_in", 
			"texture_coord_up_in",
			"texture_coord_side_in", 
			"texture_coord_down_in", 
			"face_in", 
			"texture_offset_in"
		});

    glUseProgram(shader_program);

    time_location = glGetUniformLocation(shader_program, "u_time");
    mvp_matrix_location = glGetUniformLocation(shader_program, "u_mvp_matrix");
    normal_matrix_location = glGetUniformLocation(shader_program, "u_normal_matrix");

    texture_location = glGetUniformLocation(shader_program, "u_texture1");
	light_texture_location = glGetUniformLocation(shader_program, "u_light_texture");

    light_position_location = glGetUniformLocation(shader_program, "u_light_position");
    light_color_location = glGetUniformLocation(shader_program, "u_light_color");
    camera_position_location = glGetUniformLocation(shader_program, "u_camera_position");

	shadow_light_mvp_location = glGetUniformLocation(shader_program, "u_light_mvp_matrix");

	GLuint poisson_location = glGetUniformLocation(shader_program, "u_poisson_disk");
	glUniform2fv(poisson_location, 64, &(poisson_disk[0][0]));

    glUniform3f(light_color_location, 1.0f, 1.0f, 1.0f);

    glUseProgram(0);
}

void scene_shading::compile_light_shaders() {
	light_shader_program = generateShaderProgram({
		{"shaders/depth.vert", GL_VERTEX_SHADER},
		{"shaders/depth.frag", GL_FRAGMENT_SHADER},
	}, 
	{"position_in", "offset_in"});

	light_mvp_location = glGetUniformLocation(light_shader_program, "u_light_mvp_matrix");
}

vec2 scene_shading::get_texture_coords(int x, int y) {
	x--;
	y--;
	return vec2(x / 16.0f, y / 23.0f );
}

//vector<vec2> scene_shading::get_texture_coords(int x, int y) {
    
	/*x--;
    y--;
    vec2 lower_left = {x / 16.0f, (y / 23.0f)};
    vec2 lower_right = {(x + 1.0f) / 16.0f, (y / 23.0f)};
    vec2 upper_left = {x / 16.0f, (y + 1.0f) / 23.0f};
    vec2 upper_right = {(x + 1.0f) / 16.0f, (y + 1.0f) / 23.0f};

    vector<vec2> texture_coords = {
        lower_left,
        lower_right,
        upper_right,
        upper_left,
    };
    return texture_coords;*/
//}

vector<vec3> scene_shading::cube_positions() {
    vec3 back_lower_left = {-0.5f, -0.5f, -0.5f};
    vec3 back_lower_right = {0.5f, -0.5f, -0.5f};
    vec3 back_upper_right = {0.5f, 0.5f, -0.5f};
    vec3 back_upper_left = {-0.5f, 0.5f, -0.5f};
    vec3 front_lower_left = {-0.5f, -0.5f, 0.5f};
    vec3 front_lower_right = {0.5f, -0.5f, 0.5f};
    vec3 front_upper_right = {0.5f, 0.5f, 0.5f};
    vec3 front_upper_left = {-0.5f, 0.5f, 0.5f};

    vector<vec3> positions = {
        // front
        front_lower_left,
        front_lower_right,
        front_upper_right,
        front_upper_left,
        // left
        back_lower_left,
        front_lower_left,
        front_upper_left,
        back_upper_left,
        // right
        front_lower_right,
        back_lower_right,
        back_upper_right,
        front_upper_right,
        // up
        front_upper_left,
        front_upper_right,
        back_upper_right,
        back_upper_left,
        // down
        back_lower_left,
        back_lower_right,
        front_lower_right,
        front_lower_left,
        // back
        back_lower_right,
        back_lower_left,
        back_upper_left,
        back_upper_right,
    };
    return positions;
}

//vector<vec2> scene_shading::cube_texture_coords(int id) {
//    vector<vec2> texture_coords_up =
//        get_texture_coords(textures[id].x_up, textures[id].y_up);
//    vector<vec2> texture_coords_side =
//        get_texture_coords(textures[id].x_side, textures[id].y_side);
//    vector<vec2> texture_coords_down =
//        get_texture_coords(textures[id].x_down, textures[id].y_down);
//    vector<vec2> texture_coords = {
//        // front
//        texture_coords_side[0],
//        texture_coords_side[1],
//        texture_coords_side[2],
//        texture_coords_side[3],
//        // left
//        texture_coords_side[0],
//        texture_coords_side[1],
//        texture_coords_side[2],
//        texture_coords_side[3],
//        // right
//        texture_coords_side[0],
//        texture_coords_side[1],
//        texture_coords_side[2],
//        texture_coords_side[3],
//        // up
//        texture_coords_up[0],
//        texture_coords_up[1],
//        texture_coords_up[2],
//        texture_coords_up[3],
//        // down
//        texture_coords_down[0],
//        texture_coords_down[1],
//        texture_coords_down[2],
//        texture_coords_down[3],
//        // back
//        texture_coords_side[0],
//        texture_coords_side[1],
//        texture_coords_side[2],
//        texture_coords_side[3],
//    };
//    return texture_coords;
//}

vector<vec3> scene_shading::cube_normal_vectors() {
    vec3 front = {0.0f, 0.0f, 1.0f};
    vec3 back = {0.0f, 0.0f, -1.0f};
    vec3 up = {0.0f, 1.0f, 0.0f};
    vec3 down = {0.0f, -1.0f, 0.0f};
    vec3 left = {-1.0f, 0.0f, 0.0f};
    vec3 right = {1.0f, 0.0f, 0.0f};

    vector<vec3> normal_vectors = {front, front, front, front, left,  left,
                                   left,  left,  right, right, right, right,
                                   up,    up,    up,    up,    down,  down,
                                   down,  down,  back,  back,  back,  back};

    return normal_vectors;
}

bool scene_shading::is_block_visible(int i, int j, int k) {
    
	if (k == 0) {
		return true;
	}
	
	int a_start = i == 0 ? 0 : -1;
    int a_end = i == MAP_SIZE ? 0 : 1;

    int b_start = j == 0 ? 0 : -1;
    int b_end = j == MAP_SIZE ? 0 : 1;

    int c_start = k == 0 ? 0 : -1;
    int c_end = k == MAP_HEIGHT ? 0 : 1;

	short currentBlock = blocks[i][j][k];
	short block = 0;

    for (int a = a_start; a <= a_end; a++) {
        for (int b = b_start; b <= b_end; b++) {
            for (int c = c_start; c <= c_end; c++) {
				block = blocks[i + a][j + b][k + c];
                if (block == 0) {
                    return true;
                }
				if (currentBlock != WATER_TYPE && block == WATER_TYPE) {
					return true;
				}
            }
        }
    }
    return false;
}

void scene_shading::make_near_blocks_visible(int i, int j, int k) {
	int a_start = i == 0 ? 0 : -1;
	int a_end = i == MAP_SIZE ? 0 : 1;

	int b_start = j == 0 ? 0 : -1;
	int b_end = j == MAP_SIZE ? 0 : 1;

	int c_start = k == 0 ? 0 : -1;
	int c_end = k == MAP_HEIGHT ? 0 : 1;

	int ni, nj, nk;
	
	for (int a = a_start; a <= a_end; a++) {
		for (int b = b_start; b <= b_end; b++) {
			for (int c = c_start; c <= c_end; c++) {
				ni = i + a;
				nj = j + b;
				nk = k + c;

				if (blocks[ni][nj][nk] != 0 && coords_to_offset_index[ni][nj][nk] == 0) {
					add_block(ni, nj, nk, blocks[ni][nj][nk]);
				}
			}
		}
	}
}

void scene_shading::add_block(int i, int j, int k, int type) {
	blocks[i][j][k] = type;

	texture_up_coords.push_back(get_texture_coords(textures[type - 1].x_up, textures[type - 1].y_up));
	texture_side_coords.push_back(get_texture_coords(textures[type - 1].x_side, textures[type - 1].y_side));
	texture_down_coords.push_back(get_texture_coords(textures[type - 1].x_down, textures[type - 1].y_down));

	coords_to_offset_index[i][j][k] = offsets.size();
	offsets.push_back(vec3(i + 0.5f, k + 0.5f, j + 0.5f));
}

void scene_shading::generate_tree(int i, int j, int k, int biome) {
	int leaves = 0;
	int wood = 0;
	if (biome == SNOW_BIOME) {
		leaves = SNOW_LEAVES_TYPE;
		wood = DARK_WOOD_TYPE;
	}
	else if (biome == FOREST_BIOME) {
		leaves = LEAVES_TYPE;
		wood = WOOD_TYPE;
	}
	else if (biome == PLAINS_BIOME) {
		if (randf() > 0.01) {
			return;
		}
		leaves = LEAVES_TYPE;
		wood = WOOD_TYPE;
	}
	else if (biome == DESERT_BIOME) {
		wood = CACTUS_TYPE;
	}
	else if (biome == AUTUM_FOREST) {
		wood = DARK_WOOD_TYPE;
		leaves = AUTUMN_LEAVES_TYPE;
	}
	else {
		return;
	}

	int height = int(randf() * 3.0f + 4.0f);

	for (int h = k; h < k+height; h++) {
		blocks[i][j][h] = wood;
	}
	blocks[i][j][k + height] = leaves;

	int width = height;
	if (width % 2 == 0) {
		width++;
	}
	
	int limit = width / 2;
	for (int a = -limit; a <= limit; a++) {
		for (int b = -limit; b <= limit; b++) {
			for (int h = -limit; h <= limit; h++) {
				if (i + a > 1000 || i + a < 0 ||
					j+b > 1000 || j + b < 0) {
					continue;
				}
				if (randf() < 0.7f && blocks[i+a][j+b][k+h+height] == 0) {
					blocks[i+a][j+b][k+h+height] = leaves;
				}
			}
		}
	}
}

int scene_shading::get_biome(float map_height, float biome_height) {
	if (map_height > 0.7) return SNOW_BIOME;
	if (map_height > 0.45) {
		if (biome_height < 0.3) return PLAINS_BIOME;
		if (biome_height < 0.5) return FOREST_BIOME;
		if (biome_height < 0.7) return AUTUM_FOREST;
		return DESERT_BIOME;
	}
	if(map_height > 0.44) return BEACH_BIOME;
	return OCEAN_BIOME;
}

void scene_shading::generate_map() {

	const int MAP_PERLIN_SIZE = 10;
	const int BIOME_PERLIN_SIZE = 5;
	const float MAP_PERLIN_RESOLUTION = (MAP_SIZE + 0.0f) / MAP_PERLIN_SIZE;
	const float BIOME_PERLIN_RESOLUTION = (MAP_SIZE + 0.0f) / BIOME_PERLIN_SIZE;

	perlin map_noise;
	perlin biome_noise;

    for (int i = 0; i <= MAP_SIZE; i++) {
        for (int j = 0; j <= MAP_SIZE; j++) {
            for (int k = 0; k <= MAP_HEIGHT; k++) {
                blocks[i][j][k] = 0;
            }
        }
    }
    
	map_noise.generate_grid(MAP_PERLIN_SIZE + 1, MAP_PERLIN_SIZE + 1, time(nullptr));
	biome_noise.generate_grid(BIOME_PERLIN_SIZE + 1, BIOME_PERLIN_SIZE + 1, time(nullptr));

    for (int i = 0; i <= MAP_SIZE; i++) {
        for (int j = 0; j <= MAP_SIZE; j++) {
			float map_range = (0.5f*map_noise.height_at(i / MAP_PERLIN_RESOLUTION, j / MAP_PERLIN_RESOLUTION)) + 0.5f;
			float biome_range = (0.5f*biome_noise.height_at(i / BIOME_PERLIN_RESOLUTION, j / BIOME_PERLIN_RESOLUTION)) + 0.5f;

			int height = int(MAP_HEIGHT * map_range);

			int biome = get_biome(map_range, biome_range);

			int grass;
			int dirt;

			if (biome == SNOW_BIOME) {
				grass = SNOW_TYPE;
				dirt = DIRT_TYPE;
			}
			else if (biome == FOREST_BIOME) {
				grass = GRASS_TYPE;
				dirt = DIRT_TYPE;
			}
			else if (biome == PLAINS_BIOME) {
				grass = GRASS_TYPE;
				dirt = DIRT_TYPE;
			}
			else if (biome == DESERT_BIOME || biome == BEACH_BIOME || biome == OCEAN_BIOME) {
				grass = SAND_TYPE;
				dirt = SAND_TYPE;
			}
			else if (biome == AUTUM_FOREST) {
				grass = DARK_GRASS_TYPE;
				dirt = DIRT_TYPE;
			}

            for (int k = height; k >= 0; k--) {
				 if (blocks[i][j][k + 1] == 0) {
                    blocks[i][j][k] = grass;
                } else if (blocks[i][j][k + 3] == 0) {
                    blocks[i][j][k] = dirt;
                } else {
                    blocks[i][j][k] = STONE_TYPE;
                }
            }

			for (int k = ceil(0.44f*MAP_HEIGHT); k > height; k--) {
				if (blocks[i][j][k] == 0) {
					blocks[i][j][k] = WATER_TYPE;
				}
			}
			if (randf() < 0.01) {
				generate_tree(i, j,height + 1, biome);
			}
        }
    }

	offsets.clear();
	offsets.reserve(RENDER_DISTANCE*RENDER_DISTANCE * 3);
	texture_up_coords.clear();
	texture_up_coords.reserve(RENDER_DISTANCE*RENDER_DISTANCE * 3);
	texture_side_coords.clear();
	texture_side_coords.reserve(RENDER_DISTANCE*RENDER_DISTANCE * 3);
	texture_down_coords.clear();
	texture_down_coords.reserve(RENDER_DISTANCE*RENDER_DISTANCE * 3);


	int start_x = max(int(camera_position.x - RENDER_DISTANCE), 0);
	int end_x = min(int(camera_position.x + RENDER_DISTANCE), MAP_SIZE);
	int start_z = max(int(camera_position.z - RENDER_DISTANCE), 0);
	int end_z = min(int(camera_position.z + RENDER_DISTANCE), MAP_SIZE);

	for (int k = 0; k <= MAP_HEIGHT; k++) {
		for (int i = start_x; i <= end_x; i++) {
			for (int j = start_z; j <= end_z; j++) {
				coords_to_offset_index[i][j][k] = 0;
				bool in_range = (vec3(i, k, j) - camera_position).magnitude() < RENDER_DISTANCE;
				if (in_range && blocks[i][j][k] != 0 && is_block_visible(i, j, k)) {
					add_block(i,j,k, blocks[i][j][k]);
				}
			}
		}
	}
}

void scene_shading::init() {

	init_poisson();

    vector<vec3> positions = cube_positions();
    vector<vec3> normal_vectors = cube_normal_vectors();
	generate_map();
	//update_map();

	vector<float> faces = {
		// front
		1.0f,1.0f,1.0f,1.0f,
		// left
		1.0f,1.0f,1.0f,1.0f,
		// right
		1.0f,1.0f,1.0f,1.0f,
		// up
		0.0f,0.0f,0.0f,0.0f,
		// down
		2.0f,2.0f,2.0f,2.0f,
		// back
		1.0f,1.0f,1.0f,1.0f
	};

	vector<vec2> texture_offsets = {
		// front
		{0.0f, 0.0f}, {1.0f / 16.0f, 0.0f}, {1.0f / 16.0f, 1.0f / 23.0f}, {0.0f, 1.0f / 23.0f},
		// left
		{0.0f, 0.0f}, {1.0f / 16.0f, 0.0f}, {1.0f / 16.0f, 1.0f / 23.0f}, {0.0f, 1.0f / 23.0f},
		// right
		{0.0f, 0.0f}, {1.0f / 16.0f, 0.0f}, {1.0f / 16.0f, 1.0f / 23.0f}, {0.0f, 1.0f / 23.0f},
		// up
		{0.0f, 0.0f}, {1.0f / 16.0f, 0.0f}, {1.0f / 16.0f, 1.0f / 23.0f}, {0.0f, 1.0f / 23.0f},
		// down
		{0.0f, 0.0f}, {1.0f / 16.0f, 0.0f}, {1.0f / 16.0f, 1.0f / 23.0f}, {0.0f, 1.0f / 23.0f},
		// back
		{0.0f, 0.0f}, {1.0f / 16.0f, 0.0f}, {1.0f / 16.0f, 1.0f / 23.0f}, {0.0f, 1.0f / 23.0f},
	};

    vector<unsigned int> indices = {
        // front
		0,1,2,0,2,3,
        // left
        4,5,6,4,6,7,
        // right
        8,9,10,8,10,11,
		// up
        12,13,14,12,14,15,
		// down
        16,17,18,16,18,19,
        // back
        20,21,22,20,22,23
    };

    compile_shaders();
	compile_light_shaders();

    glGenVertexArrays(1, &vao);

    indexIBO = generateIBO(vao, indices, GL_STATIC_DRAW);
    positionsVBO = generateVBO(vao, 0, positions, 3, GL_FLOAT, GL_STATIC_DRAW);
    offsets_coordsVBO = generateInstancedVBO(vao, 1, offsets, 3, GL_FLOAT, GL_STATIC_DRAW, 1);

    normal_vectorsVBO = generateVBO(vao, 2, normal_vectors, 3, GL_FLOAT, GL_STATIC_DRAW);
    
	texture_coords_up_VBO = generateInstancedVBO(vao, 3, texture_up_coords, 2, GL_FLOAT, GL_STATIC_DRAW, 1);
	texture_coords_side_VBO = generateInstancedVBO(vao, 4, texture_side_coords, 2, GL_FLOAT, GL_STATIC_DRAW, 1);
	texture_coords_down_VBO = generateInstancedVBO(vao, 5, texture_down_coords, 2, GL_FLOAT, GL_STATIC_DRAW, 1);
	texture_face_VBO = generateVBO(vao, 6, faces, 1, GL_FLOAT, GL_STATIC_DRAW);
	texture_offsets_VBO = generateVBO(vao, 7, texture_offsets, 2, GL_FLOAT, GL_STATIC_DRAW);


    textureId = generateTexture("assets/spritesheet2.png");

	buffer.create(8192);

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void scene_shading::awake() {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	double x, y;
	glfwGetCursorPos(window, &x, &y);
	mouse_x = float(x);
	mouse_y = float(y);
	glClearColor(208.0f / 255.0f, 183.0f / 255.0f, 249.0f / 255.0f, 1.0f);
}

void scene_shading::sleep() {
    glClearColor(1.0f, 1.0f, 0.5f, 1.0f);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void scene_shading::first_render() {
	buffer.bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(light_shader_program);


	glUniformMatrix4fv(light_mvp_location, 1, GL_FALSE, &((light_proyection_matrix * light_view_matrix)[0][0]));

	glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr, offsets.size());

	glUseProgram(0);
	buffer.unbind();
}

void scene_shading::second_render() {
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shader_program);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureId);

	float u_time = time::elapsed_time().count();
	glUniform1f(time_location, u_time);
	glUniform1i(texture_location, 0);
	glUniform1i(light_texture_location, 1);

	mat3 normal_matrix = mat3::transpose(mat3::inverse(mat3(0.5f)));
	glUniformMatrix3fv(normal_matrix_location, 1, GL_FALSE,
		&(normal_matrix[0][0]));
	
	glUniform3fv(camera_position_location, 1,
		&(camera_position[0]));

	glUniform3f(light_position_location, 10.0f, 20.0f, 10.0f);

	mat4 view_matrix =
		lookAt(camera_position, camera_position + forward, { 0.0f, 1.0f, 0.0f });
	glUniformMatrix4fv(mvp_matrix_location, 1, GL_FALSE, &((proyection_matrix*view_matrix)[0][0]));
	
	glUniformMatrix4fv(shadow_light_mvp_location, 1, GL_FALSE, &((light_proyection_matrix * light_view_matrix)[0][0]));

	glActiveTexture(GL_TEXTURE1);
	buffer.bindDepthMap();

	glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr, offsets.size());

	glActiveTexture(GL_TEXTURE1);
	buffer.unbindDepthMap();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}

//bool is_inside(const vec3& point, const vec3 &box) {
//	float min_x = box.x - 0.5f;
//	float min_y = box.y - 0.5f;
//	float min_z = box.z - 0.5f;
//	
//	float max_x = box.x + 0.5f;
//	float max_y = box.y + 0.5f;
//	float max_z = box.z + 0.5f;
//
//	if (min_x < point.x && point.x  < max_x &&
//		min_y < point.y && point.y  < max_y &&
//		min_z < point.z && point.z  < max_z){
//		return true;
//	}
//	return false;
//}

bool scene_shading::has_object(const vec3& v) {
	if (is_outside(v)) {
		return false;
	}
	int x = int(v.x);
	int y = int(v.y);
	int z = int(v.z);

	return blocks[x][z][y] != 0;
}

bool scene_shading::is_outside(const vec3& v) {
	if (v.x < 0 || MAP_SIZE < v.x ||
		v.y < 0 || MAP_HEIGHT < v.y ||
		v.z < 0 || MAP_SIZE < v.z) {
		return true;
	}
	return false;
}

//vector<vec3> scene_shading::voxels(const vec3& ray, const vec3& origin) {
	//vector<vec3> result;
	//if (!is_outside(origin)) {
	//	result.push_back(origin);
	//}

	//vec3 position = origin;
	//vec3 step = vec3(sign(ray.x), sign(ray.y), sign(ray.z));
	//vec3 t;
	//vec3 direction = vec3::normalize(ray);
	//vec3 current_ray;
	//vec3 delta(1.0f, 1.0f, 1.0f);

	//for (int i = 0; i < 3; i++) {
	//	if (step[i] > 0) {
	//		t[i] = abs((truncf(position[i] + step[i]) - position[i])/ direction[i]);
	//	}
	//	else if (step[i] < 0) {
	//		t[i] = abs((position[i] - abs_ceil(position[i] + step[i]))/ direction[i]);
	//	}
	//}
	//int cont = 20;
	//while (cont--) {
	//	for (int k = 0; k < 3; k++) {
	//		float t_min = t[0];
	//		int t_min_axis = 0;
	//		for (int i = 0; i < 3; i++) {
	//			if (t[i] < t_min) {
	//				t_min = t[i];
	//				t_min_axis = i;
	//			}
	//		}

	//		vec3 current_ray = t_min*direction;

	//		if(current_ray.magnitude() > 10.0f) {
	//			return result;
	//		}

	//		position[t_min_axis] += step[t_min_axis];
	//		t[t_min_axis] += delta[t_min_axis];
	//		
	//		if (!is_outside(position)) {
	//			result.push_back(position);
	//		}
	//	}
	//}

	//return result;
//}

//vector<vec3> scene_shading::voxels(const vec3& ray, const vec3& origin) {
//	vector<vec3> result;
//	
//	if (!is_outside(origin)) {
//		result.push_back(origin);
//	}
//
//	vec3 t_max, t_delta;
//	vec3 current_voxel;
//	
//	vec3 start = origin;
//	vec3 end = origin + ray;
//
//	vec3 delta = vec3(sign(ray.x), sign(ray.y), sign(ray.z));
//
//	for (int i = 0; i < 3; i++) {
//		if (ray[i] == 0.0f){
//			t_delta[i] = 100000.0f;
//		}
//		else {
//			t_delta[i] = delta[i] / (ray[i]);
//		}
//		t_max[i] = t_delta[i] * (1.0f - abs(modf(start[i], &current_voxel[i])));
//	}
//
//	while (true) {
//		float t_min = t_max[0];
//		int t_min_dimension = 0;
//		for (int i = 1; i < 3; i++) {
//			if (t_max[i] < t_min) {
//				t_min = t_max[i];
//				t_min_dimension = i;
//			}
//
//		}
//		current_voxel[t_min_dimension] += delta[t_min_dimension];
//		t_max[t_min_dimension] += t_delta[t_min_dimension];
//
//		if(t_max.x > 1 && t_max.y > 1 && t_max.z > 1) break;
//
//		result.push_back(current_voxel);
//	}
//
//	return result;
//}

vector<vec3> scene_shading::voxels(const vec3& ray, const vec3& origin) {
	vector<vec3> result;

	if (!is_outside(origin)) {
		result.push_back(origin);
	}

	vec3 t_max, t_delta;
	vec3 current_voxel = origin;
	vec3 end = origin + ray;
	end = vec3(int(end.x), int(end.y), int(end.z));

	vec3 step = vec3(sign(ray.x), sign(ray.y), sign(ray.z));

	float border;
	for (int i = 0; i < 3; i++) {
		if (step[i] > 0.0) {
			border = floor(origin[i] + 1.0);
		}
		else {
			border = ceil(origin[i] - 1.0);
		}
		if (AreEqual(ray[i], 0.0)) {
			t_max[i] = 100000.0;
			t_delta[i] = 100000.0;
		}
		else {
			t_max[i] = (border - origin[i]) / ray[i];
			t_delta[i] = step[i] / ray[i];
		}
	}

	while (true) {
		float t_min = t_max[0];
		int t_min_dimension = 0;
		for (int i = 1; i < 3; i++) {
			if (t_max[i] < t_min) {
				t_min = t_max[i];
				t_min_dimension = i;
			}
		}
		current_voxel[t_min_dimension] += step[t_min_dimension];
		t_max[t_min_dimension] += t_delta[t_min_dimension];

		//if (t_max.x > 1  && t_max.y > 1 && t_max.z > 1) break;
		if ((current_voxel - origin).magnitude() > ray.magnitude()) break;

		result.push_back(current_voxel);
	}

	return result;
}

//vector<vec3> scene_shading::voxels(const vec3& ray, const vec3& origin) {
//	vector<vec3> result;
//
//	if (!is_outside(origin)) {
//		result.push_back(origin);
//	}
//
//	vec3 t_max, t_delta;
//	vec3 current_voxel = origin;
//	vec3 end = origin + ray;
//	end = vec3(int(end.x), int(end.y), int(end.z));
//
//	vec3 step = vec3(sign(ray.x), sign(ray.y), sign(ray.z));
//
//	float border;
//	for (int i = 0; i < 3; i++) {
//		if (step[i] > 0.0f) {
//			border = floor(origin[i] + 1.0f);
//		}
//		else {
//			border = ceil(origin[i] - 1.0f);
//		}
//		if (AreEqual(ray[i], 0.0f)) {
//			t_max[i] = 100000.0f;
//			t_delta[i] = 100000.0f;
//		}
//		else {
//			t_max[i] = (border - origin[i]) / ray[i];
//			t_delta[i] = step[i] / ray[i];
//		}
//	}
//
//	vec3 current_voxel_int;
//	while (true) {
//		float t_min = t_max[0];
//		int t_min_dimension = 0;
//		for (int i = 1; i < 3; i++) {
//			if (t_max[i] < t_min) {
//				t_min = t_max[i];
//				t_min_dimension = i;
//			}
//		}
//		current_voxel = origin + (t_min * ray);
//		t_max[t_min_dimension] += t_delta[t_min_dimension];
//
//		if (t_max.x > 1 && t_max.y > 1 && t_max.z > 1) break;
//		//current_voxel_int = vec3(int(current_voxel.x), int(current_voxel.y), int(current_voxel.z));
//		//if (current_voxel == end) {
//		//	break;
//		//}
//
//		result.push_back(current_voxel);
//	}
//
//	return result;
//}

void scene_shading::update_map(std::atomic<bool>& program_is_running) {
	
	vector<vec3> m_offsets;
	vector<vec2> m_texture_up_coords;
	vector<vec2> m_texture_side_coords;
	vector<vec2> m_texture_down_coords;

	int start_x, end_x;
	int start_z, end_z;
	bool in_range;
	int i, j, k, type;

	while (program_is_running) {

		m_offsets.clear();
		m_texture_up_coords.clear();
		m_texture_side_coords.clear();
		m_texture_down_coords.clear();

		m_offsets.reserve(RENDER_DISTANCE*RENDER_DISTANCE * 3);
		m_texture_up_coords.reserve(RENDER_DISTANCE*RENDER_DISTANCE * 3);
		m_texture_side_coords.reserve(RENDER_DISTANCE*RENDER_DISTANCE * 3);
		m_texture_down_coords.reserve(RENDER_DISTANCE*RENDER_DISTANCE * 3);

		start_x = max(int(camera_position.x - RENDER_DISTANCE), 0);
		end_x = min(int(camera_position.x + RENDER_DISTANCE), MAP_SIZE);
		start_z = max(int(camera_position.z - RENDER_DISTANCE), 0);
		end_z = min(int(camera_position.z + RENDER_DISTANCE), MAP_SIZE);
		
		for (k = 0; k <= MAP_HEIGHT; k++) {
			for (i = start_x; i <= end_x; i++) {
				for (j = start_z; j <= end_z; j++) {
					in_range = (vec3(i, 0.0f, j) - vec3(camera_position.x, 0.0f, camera_position.z)).magnitude() < RENDER_DISTANCE;

					coords_to_offset_index[i][j][k] = 0;
					if (in_range && blocks[i][j][k] != 0 && is_block_visible(i, j, k)) {
						type = blocks[i][j][k] - 1;

						m_texture_up_coords.push_back(get_texture_coords(textures[type].x_up, textures[type].y_up));
						m_texture_side_coords.push_back(get_texture_coords(textures[type].x_side, textures[type].y_side));
						m_texture_down_coords.push_back(get_texture_coords(textures[type].x_down, textures[type].y_down));

						coords_to_offset_index[i][j][k] = m_offsets.size();
						m_offsets.push_back(vec3(i + 0.5f, k + 0.5f, j + 0.5f));
					}
				}
			}
		}

		std::cout << "updated" << std::endl;

		offsets = m_offsets;
		texture_up_coords = m_texture_up_coords;
		texture_side_coords = m_texture_side_coords;
		texture_down_coords = m_texture_down_coords;
	}
}

void scene_shading::mainLoop() {
    handle_movement();
    handle_rotation();
    handle_gravity();
	handle_raycasting();

	light_view_matrix = lookAt(light_camera_position, { camera_position.x, 0.0f, camera_position.z }, { 0.0f, 1.0f, 0.0f });

    glBindVertexArray(vao);

	updateInstancedVBO(vao, offsets_coordsVBO, 1, offsets, 3, GL_FLOAT, GL_STATIC_DRAW, 1);
	updateInstancedVBO(vao, texture_coords_up_VBO, 3, texture_up_coords, 2, GL_FLOAT, GL_STATIC_DRAW, 1);
	updateInstancedVBO(vao, texture_coords_side_VBO, 4, texture_side_coords, 2, GL_FLOAT, GL_STATIC_DRAW, 1);
	updateInstancedVBO(vao, texture_coords_down_VBO, 5, texture_down_coords, 2, GL_FLOAT, GL_STATIC_DRAW, 1);

	first_render();
	second_render();

    glBindVertexArray(0);
}

void scene_shading::resize(int width, int height) {
    this->width = width;
    this->height = height;

    glViewport(0, 0, width, height);

    glUseProgram(shader_program);

    proyection_matrix = mat4::transpose({
        {1.0f / (((width + 0.0f) / (height + 0.0f)) * tan(radians(60.0f) / 2.0f)),
         0.0f, 0.0f, 0.0f},
        {0.0f, 1.0f / (tan(radians(60.0f) / 2.0f)), 0.0f, 0.0f},
        {0.0f, 0.0f, -(1000.0f + 1.0f) / (1000.0f - 1.0f),
         -(2.0f * 1000.0f * 1.0f) / (1000.0f - 1.0f)},
        {0.0f, 0.0f, -1.0f, 0.0f},
    });

    glUseProgram(0);
    this->width = width;
    this->height = height;
}

void scene_shading::handle_gravity() {
    camera_position.y += 10.0f * up_input * time::delta_time().count();
}

void scene_shading::handle_rotation() {
    float delta_time = time::delta_time().count();

	yaw_input = delta_x;
	pitch_input = delta_y;

	delta_x = 0;
	delta_y = 0;

    if (abs(yaw_input) < 0.05f && abs(pitch_input) < 0.05f) {
        return;
    }

    float rotation_speed = radians(delta_time);
    vec2 rotation = rotation_speed * vec2(yaw_input, pitch_input);

	const vec3 UP = { 0.0f, 1.0f, 0.0f };
	mat4 yaw_rotation_matrix = rotate_around(rotation.x, UP);
	mat4 pitch_rotation_matrix = rotate_around(rotation.y, right);

	vec3 new_forward = pitch_rotation_matrix * vec4(forward.x, forward.y, forward.z, 1.0f);
	vec3 front = vec3::normalize(vec3::cross(UP, right));

	float temp_dot = vec3::dot(new_forward, front);
	float temp_dot2 = vec3::dot(UP, new_forward);
	if (acosf(vec3::dot(new_forward, front)) > radians(89.0f)) {
		bool is_looking_up = vec3::dot(UP, new_forward) > 0.0f;
		float _radians = is_looking_up ? radians(89.0f): radians(-89.0f);
		forward = yaw_rotation_matrix * rotate_around(_radians, right) * vec4(front.x, front.y, front.z, 1.0f);
	
	}
	else {
		forward = yaw_rotation_matrix * pitch_rotation_matrix * vec4(forward.x, forward.y, forward.z, 1.0f);
	}

	forward = vec3::normalize(forward);
    right = vec3::normalize(vec3::cross(forward, UP));
    upward = vec3::normalize(vec3::cross(right, forward));
}

void scene_shading::handle_movement() {
    float delta_time = time::delta_time().count();

    if (horizontal_input == 0 && vertical_input == 0 ) {
        return;
    }

    float speed = 10.0f * delta_time;
    vec2 movement =
        speed * vec2::normalize(vec2(float(horizontal_input), float(vertical_input)));

	vec3 front = vec3::normalize(vec3::cross({ 0.0f, 1.0f, 0.0f }, right));

	camera_position.x += (right * movement.x).x + (front * movement.y).x;
	camera_position.z += (right * movement.x).z + (front * movement.y).z;

	light_camera_position.x += (right * movement.x).x + (front * movement.y).x;
	light_camera_position.z += (right * movement.x).z + (front * movement.y).z;
}

void scene_shading::handle_add_block() {
	vector<vec3> _voxels = voxels(forward * 10.0f, camera_position);
	vec3 prev_voxel;
	for (auto &voxel : _voxels) {
		if (has_object(voxel)) {
			int x = int(prev_voxel.x);
			int y = int(prev_voxel.y);
			int z = int(prev_voxel.z);

			add_block(x, z, y, WOOD_TYPE);
			return;
		}
		else {
			prev_voxel = voxel;
		}
	}
}

void scene_shading::handle_remove_block() {
	vector<vec3> _voxels = voxels(forward * 10.0f, camera_position);
	for (auto &voxel : _voxels) {
		if (has_object(voxel)) {
			int x = int(voxel.x);
			int y = int(voxel.y);
			int z = int(voxel.z);

			make_near_blocks_visible(x, z, y);

			blocks[x][z][y] = 0;

			int nx = int(offsets[offsets.size() - 1].x);
			int ny = int(offsets[offsets.size() - 1].y);
			int nz = int(offsets[offsets.size() - 1].z);

			int index = coords_to_offset_index[x][z][y];
			coords_to_offset_index[nx][nz][ny] = index;
			coords_to_offset_index[x][z][y] = 0;

			offsets[index] = offsets[offsets.size() - 1];
			offsets.pop_back();

			texture_up_coords[index] = texture_up_coords[texture_up_coords.size() - 1];
			texture_up_coords.pop_back();

			texture_side_coords[index] = texture_side_coords[texture_side_coords.size() - 1];
			texture_side_coords.pop_back();

			texture_down_coords[index] = texture_down_coords[texture_down_coords.size() - 1];
			texture_down_coords.pop_back();



			return;
		}
	}
}

void scene_shading::handle_raycasting() {

	if (mouse_input == 0) {
		return;
	}
	if (mouse_input == 1) {
		handle_remove_block();
	}
	if (mouse_input == 2) {
		handle_add_block();
	}

	mouse_input = 0;
}

void scene_shading::keysDown(int key) {
    if (key == GLFW_KEY_C) {
        compile_shaders();
        compile_light_shaders();
    }
    if (key == GLFW_KEY_W) {
        vertical_input = 1;
    }
    if (key == GLFW_KEY_A) {
        horizontal_input = -1;
    }
    if (key == GLFW_KEY_S) {
        vertical_input = -1;
    }
    if (key == GLFW_KEY_D) {
        horizontal_input = 1;
    }
    if (key == GLFW_KEY_Z) {
        up_input = 1;
    }
    if (key == GLFW_KEY_X) {
        up_input = -1;
    }
    /*if (key == GLFW_KEY_UP) {
        pitch_input = -1;
    }
    if (key == GLFW_KEY_DOWN) {
        pitch_input = 1;
    }
    if (key == GLFW_KEY_RIGHT) {
        yaw_input = 1;
    }
    if (key == GLFW_KEY_LEFT) {
        yaw_input = -1;
    }*/
}

void scene_shading::keysUp(int key) {
    if (key == GLFW_KEY_W && vertical_input == 1) {
        vertical_input = 0;
    }
    if (key == GLFW_KEY_A && horizontal_input == -1) {
        horizontal_input = 0;
    }
    if (key == GLFW_KEY_S && vertical_input == -1) {
        vertical_input = 0;
    }
    if (key == GLFW_KEY_D && horizontal_input == 1) {
        horizontal_input = 0;
    }
    if (key == GLFW_KEY_Z && up_input == 1) {
        up_input = 0;
    }
    if (key == GLFW_KEY_X && up_input == -1) {
        up_input = 0;
    }
    if (key == GLFW_KEY_LEFT_CONTROL) {
        capture_mouse = !capture_mouse;
        if (capture_mouse) {
			double x;
			double y;
			glfwGetCursorPos(window, &x, &y);
			mouse_x = float(x);
			mouse_y = float(y);
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        } else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }
}

void scene_shading::passiveMotion(float x, float y) {
	if (!capture_mouse) {
		return;
	}
	delta_x = (mouse_x - x);
	delta_y = (mouse_y - y);
	mouse_x = x;
	mouse_y = y;
}

void scene_shading::mouseButton(int button, int action) {
	if (action == GLFW_PRESS) {
		if (button == GLFW_MOUSE_BUTTON_LEFT) {
			std::cout << "left clicked!" << std::endl;
			mouse_input = 1;
		}
		if (button == GLFW_MOUSE_BUTTON_RIGHT) {
			std::cout << "right clicked!" << std::endl;
			mouse_input = 2;
		}
	}
}

void scene_shading::init_poisson() {
	poisson_disk[0] = vec2(-0.613392f, 0.617481f);
	poisson_disk[1] = vec2(0.170019f, -0.040254f);
	poisson_disk[2] = vec2(-0.299417f, 0.791925f);
	poisson_disk[3] = vec2(0.645680f, 0.493210f);
	poisson_disk[4] = vec2(-0.651784f, 0.717887f);
	poisson_disk[5] = vec2(0.421003f, 0.027070f);
	poisson_disk[6] = vec2(-0.817194f, -0.271096f);
	poisson_disk[7] = vec2(-0.705374f, -0.668203f);
	poisson_disk[8] = vec2(0.977050f, -0.108615f);
	poisson_disk[9] = vec2(0.063326f, 0.142369f);
	poisson_disk[10] = vec2(0.203528f, 0.214331f);
	poisson_disk[11] = vec2(-0.667531f, 0.326090f);
	poisson_disk[12] = vec2(-0.098422f, -0.295755f);
	poisson_disk[13] = vec2(-0.885922f, 0.215369f);
	poisson_disk[14] = vec2(0.566637f, 0.605213f);
	poisson_disk[15] = vec2(0.039766f, -0.396100f);
	poisson_disk[16] = vec2(0.751946f, 0.453352f);
	poisson_disk[17] = vec2(0.078707f, -0.715323f);
	poisson_disk[18] = vec2(-0.075838f, -0.529344f);
	poisson_disk[19] = vec2(0.724479f, -0.580798f);
	poisson_disk[20] = vec2(0.222999f, -0.215125f);
	poisson_disk[21] = vec2(-0.467574f, -0.405438f);
	poisson_disk[22] = vec2(-0.248268f, -0.814753f);
	poisson_disk[23] = vec2(0.354411f, -0.887570f);
	poisson_disk[24] = vec2(0.175817f, 0.382366f);
	poisson_disk[25] = vec2(0.487472f, -0.063082f);
	poisson_disk[26] = vec2(-0.084078f, 0.898312f);
	poisson_disk[27] = vec2(0.488876f, -0.783441f);
	poisson_disk[28] = vec2(0.470016f, 0.217933f);
	poisson_disk[29] = vec2(-0.696890f, -0.549791f);
	poisson_disk[30] = vec2(-0.149693f, 0.605762f);
	poisson_disk[31] = vec2(0.034211f, 0.979980f);
	poisson_disk[32] = vec2(0.503098f, -0.308878f);
	poisson_disk[33] = vec2(-0.016205f, -0.872921f);
	poisson_disk[34] = vec2(0.385784f, -0.393902f);
	poisson_disk[35] = vec2(-0.146886f, -0.859249f);
	poisson_disk[36] = vec2(0.643361f, 0.164098f);
	poisson_disk[37] = vec2(0.634388f, -0.049471f);
	poisson_disk[38] = vec2(-0.688894f, 0.007843f);
	poisson_disk[39] = vec2(0.464034f, -0.188818f);
	poisson_disk[40] = vec2(-0.440840f, 0.137486f);
	poisson_disk[41] = vec2(0.364483f, 0.511704f);
	poisson_disk[42] = vec2(0.034028f, 0.325968f);
	poisson_disk[43] = vec2(0.099094f, -0.308023f);
	poisson_disk[44] = vec2(0.693960f, -0.366253f);
	poisson_disk[45] = vec2(0.678884f, -0.204688f);
	poisson_disk[46] = vec2(0.001801f, 0.780328f);
	poisson_disk[47] = vec2(0.145177f, -0.898984f);
	poisson_disk[48] = vec2(0.062655f, -0.611866f);
	poisson_disk[49] = vec2(0.315226f, -0.604297f);
	poisson_disk[50] = vec2(-0.780145f, 0.486251f);
	poisson_disk[51] = vec2(-0.371868f, 0.882138f);
	poisson_disk[52] = vec2(0.200476f, 0.494430f);
	poisson_disk[53] = vec2(-0.494552f, -0.711051f);
	poisson_disk[54] = vec2(0.612476f, 0.705252f);
	poisson_disk[55] = vec2(-0.578845f, -0.768792f);
	poisson_disk[56] = vec2(-0.772454f, -0.090976f);
	poisson_disk[57] = vec2(0.504440f, 0.372295f);
	poisson_disk[58] = vec2(0.155736f, 0.065157f);
	poisson_disk[59] = vec2(0.391522f, 0.849605f);
	poisson_disk[60] = vec2(-0.620106f, -0.328104f);
	poisson_disk[61] = vec2(0.789239f, -0.419965f);
	poisson_disk[62] = vec2(-0.545396f, 0.538133f);
	poisson_disk[63] = vec2(-0.178564f, -0.596057f);
}