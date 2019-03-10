#pragma once

#include "scene.h"

class scene_fragment : public scene
{
public:

	~scene_fragment();

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
	GLuint indicesBuffer;

	int width= 200;
	int height = 200;

	void compile_shaders();
};
