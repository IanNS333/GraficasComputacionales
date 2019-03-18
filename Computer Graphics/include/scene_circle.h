#pragma once

#include "scene.h"

class scene_circle : public scene
{
public:

	~scene_circle();

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
	GLuint shader_program;

	GLuint vao;
	GLuint positionsVBO;
	GLuint colorsVBO;

	GLuint time_location;
	GLuint resolution_location;

	int width = 400;
	int height = 400;
	void compile_shaders();
};
