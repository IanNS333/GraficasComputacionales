#pragma once

#include "scene.h"
#include "vec2.h"

using cgmath::vec2;

class scene_conchoid : public scene
{
public:
	void init();
	void awake();
	void sleep() {}
	void reset() {}
	void mainLoop();
	void resize(int width, int height) {}
	void normalKeysDown(unsigned char key);
	void normalKeysUp(unsigned char key) {}
	void specialKeys(int key) {}
	void passiveMotion(int x, int y) {}

	void sendVertexData();
	void conchoid();

private:
	float distance = 1.0f;
	vec2 start = { -2.0f, 0.25f };
	vec2 end = { 2.0f, 0.25f };
	vec2 origin = { 0, 0};

	GLuint vaos[2];
	GLuint positionsVBOs[2];

	std::vector<vec2> points[2];
};