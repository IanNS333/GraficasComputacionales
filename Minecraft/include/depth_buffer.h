#pragma once

#include <glad/glad.h>

class depth_buffer {

private:
	GLuint _framebuffer = 0;
	GLuint _depthmap = 0;
	GLuint _resolution = 0;

	int _prev_width = 0;
	int _prev_height = 0;

public:
	depth_buffer();
	~depth_buffer();

	void create(int resolution);
	void bind();
	void unbind();
	void bindDepthMap();
	void unbindDepthMap();
};