#pragma once

#include <gl/GL.h>

class depth_buffer {

private:
	GLuint

public:
	depth_buffer();
	~depth_buffer();

	void create(int resolution);
	void bind();
	void unbind();
	void bindDepthMap();
	void unbindDepthMap();
};