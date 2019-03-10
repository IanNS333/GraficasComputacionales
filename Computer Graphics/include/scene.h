#pragma once

#include <GL/glew.h>
#include <vector>
#include <string>

struct shader_file {
	shader_file(std::string path, GLenum type) : path(path), type(type) {}
	std::string path;
	GLenum type;
};

// Scene is an abstract class.
class scene
{
public:
	virtual ~scene() {}

	virtual void init() = 0;
	virtual void awake() = 0;
	virtual void sleep() = 0;
	virtual void reset() = 0;
	virtual void mainLoop() = 0;
	virtual void resize(int width, int height) = 0;
	virtual void normalKeysDown(unsigned char key) = 0;
	virtual void normalKeysUp(unsigned char key) = 0;
	virtual void specialKeys(int key) = 0;
	virtual void passiveMotion(int x, int y) = 0;

	GLuint generateShaderProgram(std::vector<shader_file> shaders);
};