#pragma once

#include "scene.h"
#include "mat4.h"
#include "vec2.h"
#include "vec3.h"
#include "cgmath.h"

using cgmath::radians;
using cgmath::mat4;
using cgmath::vec2;
using cgmath::vec3;
using std::vector;

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

	void init();
	void awake();
	void sleep();
	void reset() {}
	void mainLoop();
	void resize(int width, int height);
	void normalKeysDown(unsigned char key);
	void normalKeysUp(unsigned char key);
	void specialKeysUp(int key);
	void specialKeysDown(int key);
	void passiveMotion(int x, int y);

private:

	void compile_shaders();
	mat4 calculate_model_matrix(float time);
	//mat4 get_camera_model_matrix(float time);
	vector<vec2> get_texture_coords(int x, int y);
	vector<vec2> cube_texture_coords(int id);
	vector<vec3> cube_normal_vectors();
	vector<vec3> cube_positions();
	vector<vector<vec3>> generate_map();
	void handle_movement();
	void handle_rotation();
	void handle_gravity();
	bool is_block_visible(int i, int j, int k);

	static const int texture_count = 3;

	GLuint shader_program;

	GLuint vao;
	GLuint positionsVBO;
	GLuint texture_coordsVBO;
	GLuint normal_vectorsVBO;
	GLuint offsets_coordsVBO;
	GLuint indexIBO;

	GLuint time_location;
	GLuint light_color_location;
	GLuint light_position_location;
	GLuint view_matrix_location;
	GLuint proyection_matrix_location;
	GLuint normal_matrix_location;
	GLuint model_matrix_location;
	GLuint camera_position_location;

	GLuint texture_location;

	ILuint imageId;
	GLuint textureId;

	int width = 400;
	int height = 400;

	const sheet_texture GRASS_TEXTURE = { 8, 11, 7, 10, 7, 9 };
	const sheet_texture DIRT_TEXTURE = { 7, 9, 7, 9, 7, 9 };
	const sheet_texture STONE_TEXTURE = { 10, 18, 10, 18, 10, 18 };
	
	vector<vector<vec3>> offsets;
	short blocks[1001][1001][65];
	int instance_count_per_block[texture_count] = { 0, 0 };
	sheet_texture textures[texture_count] = {
		GRASS_TEXTURE,
		DIRT_TEXTURE,
		STONE_TEXTURE,
	};

	vec3 camera_position = { 0.0f, 10.0f, 10.0f };


	mat4 proyection_matrix = mat4::transpose({
		{1.0f / (1.0f * tan(radians(60.0f) / 2.0f)), 0.0f, 0.0f, 0.0f},
		{0.0f, 1.0f / (tan(radians(60.0f) / 2.0f)), 0.0f, 0.0f},
		{0.0f, 0.0f, -(1000.0f + 1.0f) / (1000.0f - 1.0f), -(2.0f * 1000.0f * 1.0f) / (1000.0f - 1.0f)},
		{0.0f, 0.0f, -1.0f, 0.0f},
	});

	//float vertical_movement;
	//float horizontal_movement;
	int vertical_input = 0;
	int horizontal_input = 0;
	int up_input = 0;
	int yaw_input = 0;
	int pitch_input = 0;
	
	vec3 right = { 1.0f, 0.0f, 0.0f };
	vec3 upward = { 0.0f, 1.0f, 0.0f };
	vec3 forward = vec3::normalize({ 0.0f, -1.0f, -1.0f });

	/*cgmath::mat4 proyection_matrix = mat4::transpose({
		{2.0f/5.0f, 0.0f, 0.0f, 0.0f},
		{0.0f, 2.0f/5.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 2.0/5.0f, 0.0f},
		{0.0f, 0.0f, 0.0f, 1.0f},
	});*/
};
