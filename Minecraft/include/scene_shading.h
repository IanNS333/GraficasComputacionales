#pragma once

#include <forward_list>
#include <vector>
#include <thread>
#include <atomic>

#include "scene.h"
#include "mat4.h"
#include "vec2.h"
#include "vec3.h"
#include "cgmath.h"
#include "depth_buffer.h"

using cgmath::radians;
using cgmath::mat4;
using cgmath::vec2;
using cgmath::vec3;
using std::vector;
using std::forward_list;
using std::thread;
using std::atomic;

struct sheet_texture {
	int x_up;
	int y_up;
	int x_side;
	int y_side;
	int x_down;
	int y_down;
	sheet_texture(int x_up,
	int y_up,
	int x_side,
	int y_side,
	int x_down,
	int y_down):x_up(x_up),
		y_up(y_up),
		x_side(x_side),
		y_side(y_side),
		x_down(x_down),
		y_down(y_down){ }
};

class scene_shading : public scene
{
public:

	~scene_shading();
	scene_shading(GLFWwindow *window): scene(window){

	}
	void init();
	void awake();
	void sleep();
	void reset() {}
	void mainLoop();
	void resize(int width, int height);
	void keysDown(int key);
	void keysUp(int key);
	void passiveMotion(float x, float y);
	void mouseButton(int button, int action);

	void update_map(std::atomic<bool>& program_is_running);

private:

	void compile_shaders();
	void compile_light_shaders();
	vec2 get_texture_coords(int x, int y);
	//vector<vec2> cube_texture_coords(int id);
	vector<vec3> cube_normal_vectors();
	vector<vec3> cube_positions();
	void generate_map();
	void generate_tree(int i, int j, int k, int biome);
	int get_biome(float map_height, float biome_height);
	void handle_movement();
	void handle_rotation();
	void handle_gravity();
	void handle_raycasting();
	void handle_add_block();
	void handle_remove_block();
	void add_block(int i, int j, int k, int type);
	bool is_block_visible(int i, int j, int k);
	void make_near_blocks_visible(int i, int j, int k);
	void first_render();
	void second_render();
	void init_poisson();
	bool has_object(const vec3 &v);
	bool is_outside(const vec3 &v);
	vector<vec3> voxels(const vec3 &ray, const vec3& origin);
	static const int MAP_SIZE = 1000;
	static const int MAP_HEIGHT = 128;
	static const int RENDER_DISTANCE = 200;
	static const int texture_count = 14;

	GLuint shader_program;
	GLuint light_shader_program;

	GLuint vao;
	GLuint positionsVBO;
	GLuint texture_coords_up_VBO;
	GLuint texture_coords_side_VBO;
	GLuint texture_coords_down_VBO;
	GLuint texture_face_VBO;
	GLuint texture_offsets_VBO;
	GLuint normal_vectorsVBO;
	GLuint offsets_coordsVBO;
	GLuint indexIBO;

	GLuint time_location;
	GLuint light_color_location;
	GLuint light_position_location;
	GLuint mvp_matrix_location;
	GLuint normal_matrix_location;
	GLuint camera_position_location;
	GLuint shadow_light_mvp_location;

	GLuint light_mvp_location;

	GLuint texture_location;
	GLuint light_texture_location;

	ILuint imageId;
	GLuint textureId;

	int width = 400;
	int height = 400;

	const sheet_texture GRASS_TEXTURE = { 8, 11, 7, 10, 7, 9 };
	const sheet_texture DARK_GRASS_TEXTURE = { 10, 11, 10, 10, 7, 9 };
	const sheet_texture SNOW_TEXTURE = { 11, 11, 11, 10, 7, 9 };
	const sheet_texture DIRT_TEXTURE = { 7, 9, 7, 9, 7, 9 };
	const sheet_texture SAND_TEXTURE = { 8, 9, 8, 9, 8, 9 };
	const sheet_texture STONE_TEXTURE = { 10, 18, 10, 18, 10, 18 };
	const sheet_texture WOOD_TEXTURE = { 1, 9, 1, 10, 1, 9 };
	const sheet_texture DARK_WOOD_TEXTURE = { 3, 9, 3, 10, 3, 9 };
	const sheet_texture CACTUS_TEXTURE = { 16, 8, 16, 7, 16, 6 };
	const sheet_texture IRON_TEXTURE = { 8, 16, 8, 16, 8, 16 };
	const sheet_texture LEAVES_TEXTURE = { 1, 13, 1, 13, 1, 13 };
	const sheet_texture AUTUMN_LEAVES_TEXTURE = { 2, 13, 2, 13, 2, 13 };
	const sheet_texture SNOW_LEAVES_TEXTURE = { 11, 11, 3, 13, 3, 13 };
	const sheet_texture WATER_TEXTURE = { 4, 19, 5, 19, 5, 19 };

	vector<vec2> texture_up_coords;
	vector<vec2> texture_side_coords;
	vector<vec2> texture_down_coords;

	vector<vec3> offsets;

	int coords_to_offset_index[MAP_SIZE + 1][MAP_SIZE + 1][MAP_HEIGHT + 1];

	short blocks[MAP_SIZE + 1][MAP_SIZE + 1][MAP_HEIGHT + 1];
	sheet_texture textures[texture_count] = {
		GRASS_TEXTURE,
		DARK_GRASS_TEXTURE,
		SNOW_TEXTURE,
		DIRT_TEXTURE,
		SAND_TEXTURE,
		STONE_TEXTURE,
		WOOD_TEXTURE,
		DARK_WOOD_TEXTURE,
		CACTUS_TEXTURE,
		IRON_TEXTURE,
		//transparent
		LEAVES_TEXTURE,
		AUTUMN_LEAVES_TEXTURE,
		SNOW_LEAVES_TEXTURE,
		WATER_TEXTURE,
	};

	int GRASS_TYPE = 1;
	int DARK_GRASS_TYPE = 2;
	int SNOW_TYPE = 3;
	int DIRT_TYPE = 4;
	int SAND_TYPE = 5;
	int STONE_TYPE = 6;
	int WOOD_TYPE = 7;
	int DARK_WOOD_TYPE = 8;
	int CACTUS_TYPE = 9;
	int IRON_TYPE = 10;
	int LEAVES_TYPE = 11;
	int AUTUMN_LEAVES_TYPE = 12;
	int SNOW_LEAVES_TYPE = 13;
	int WATER_TYPE = 14;

	int PLAINS_BIOME = 1;
	int FOREST_BIOME = 2;
	int SNOW_BIOME = 3;
	int DESERT_BIOME = 4;
	int BEACH_BIOME = 5;
	int OCEAN_BIOME = 6;
	int AUTUM_FOREST = 7;

	vec3 camera_position = { 500.0f, 100.0f, 500.0f };
	vec3 light_camera_position = vec3::normalize({ 500.0f, 100.0f, 500.0f })*1000.0f;

	mat4 proyection_matrix = mat4::transpose({
		{1.0f / (1.0f * tan(radians(60.0f) / 2.0f)), 0.0f, 0.0f, 0.0f},
		{0.0f, 1.0f / (tan(radians(60.0f) / 2.0f)), 0.0f, 0.0f},
		{0.0f, 0.0f, -(1000.0f + 0.5f) / (1000.0f - 0.5f), -(2.0f * 1000.0f * 0.5f) / (1000.0f - 0.5f)},
		{0.0f, 0.0f, -1.0f, 0.0f},
	});

	/*mat4 proyection_matrix = mat4::transpose({
		{2.0f / (5.0f + 5.0f), 0.0f, 0.0f, 0.0f},
		{0.0f, 2.0f / (5.0f + 5.0f), 0.0f, 0.0f},
		{0.0f, 0.0f, -2.0f / (1000.0f), -(1000.0f) / (1000.0f)},
		{0.0f, 0.0f, 0.0f, 1.0f},
	});*/
	
	mat4 light_proyection_matrix = mat4::transpose({
		{2.0f / (RENDER_DISTANCE + RENDER_DISTANCE), 0.0f, 0.0f, 0.0f},
		{0.0f, 2.0f / (200.0f + 100.0f), 0.0f, -(250.0f - 100.0f)/(250.0f + 100.0f)},
		{0.0f, 0.0f, -2.0f / (1000.0f), - (1000.0f) / (1000.0f)},
		{0.0f, 0.0f, 0.0f, 1.0f},
	});

	mat4 light_view_matrix;
	//float vertical_movement;
	//float horizontal_movement;
	int vertical_input = 0;
	int horizontal_input = 0;
	int up_input = 0;
	float yaw_input = 0;
	float pitch_input = 0;
	int mouse_input = 0;
	
	bool capture_mouse = true;
    float mouse_x, delta_x;
    float mouse_y, delta_y;

	vec3 right = { 1.0f, 0.0f, 0.0f };
	vec3 upward = { 0.0f, 1.0f, 0.0f };
	vec3 forward = vec3::normalize({ 0.0f, -1.0f, -1.0f });


	depth_buffer buffer;

	/*cgmath::mat4 proyection_matrix = mat4::transpose({
		{2.0f/5.0f, 0.0f, 0.0f, 0.0f},
		{0.0f, 2.0f/5.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 2.0/5.0f, 0.0f},
		{0.0f, 0.0f, 0.0f, 1.0f},
	});*/

	vec2 poisson_disk[64];
};
