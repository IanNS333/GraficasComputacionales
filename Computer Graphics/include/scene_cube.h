#pragma once

#include "scene.h"
#include "mat4.h"
#include "cgmath.h"

using cgmath::radians;
using cgmath::mat4;

class scene_cube : public scene
{
public:

	~scene_cube();

	void init();
	void awake();
	void sleep();
	void reset() {}
	void mainLoop();
	void resize(int width, int height);
	void normalKeysDown(unsigned char key);
	void normalKeysUp(unsigned char key) {}
	void specialKeys(int key) {}
	void passiveMotion(int x, int y) {}

private:
	
	void compile_shaders();
	mat4 model_matrix(float time);

	GLuint shader_program;

	GLuint vao;
	GLuint positionsVBO;
	GLuint colorsVBO;
	GLuint indexVBO;

	GLuint time_location;
	GLuint transformation_matrix_location;

	int width = 400;
	int height = 400;

	mat4 view_matrix = mat4::transpose({
		{1.0f, 0.0f, 0.0f, 0.0f},
		{0.0f, 1.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 1.0f, 10.0f},
		{0.0f, 0.0f, 0.0f, 1.0f},
	});

	mat4 proyection_matrix = mat4::transpose({
		{1.0f / (1.0f * tan(radians(60.0f) / 2.0f)), 0.0f, 0.0f, 0.0f},
		{0.0f, 1.0f / (tan(radians(60.0f) / 2.0f)), 0.0f, 0.0f},
		{0.0f, 0.0f, -(1000.0f + 1.0f) / (1000.0f - 1.0f), -(2.0f * 1000.0f * 1.0f) / (1000.0f - 1.0f)},
		{0.0f, 0.0f, -1.0f, 0.0f},
	});

	/*cgmath::mat4 proyection_matrix = mat4::transpose({
		{2.0f/5.0f, 0.0f, 0.0f, 0.0f},
		{0.0f, 2.0f/5.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 2.0/5.0f, 0.0f},
		{0.0f, 0.0f, 0.0f, 1.0f},
	});*/
};
