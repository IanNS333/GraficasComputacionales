#include "scene_circle_grid.h"

#include "time_utils.h"

scene_circle_grid::~scene_circle_grid()
{
	glDeleteProgram(shader_program);
}

void scene_circle_grid::compile_shaders()
{
	shader_program = generateShaderProgram({
		{ "shaders/circle_grid.vert", GL_VERTEX_SHADER},
		{ "shaders/solid_color.frag", GL_FRAGMENT_SHADER },
	});
}

void scene_circle_grid::init()
{
	compile_shaders();
}

void scene_circle_grid::awake()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_PROGRAM_POINT_SIZE);
}

void scene_circle_grid::sleep()
{
	glClearColor(1.0f, 1.0f, 0.5f, 1.0f);
	glDisable(GL_PROGRAM_POINT_SIZE);
}

void scene_circle_grid::mainLoop()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(shader_program);

	GLuint time_location = glGetUniformLocation(shader_program, "u_time");
	glUniform1f(time_location, time::elapsed_time().count());

	for (int i = 0; i < 5000; i += 42) {
		glDrawArrays(GL_TRIANGLE_STRIP, i, 42);
	}

	glUseProgram(0);
}

void scene_circle_grid::resize(int width, int height)
{
}

void scene_circle_grid::normalKeysDown(unsigned char key)
{
	if (key == 'c') {
		compile_shaders();
	}
}