#include "scene_vertex.h"

#include "time_utils.h"

scene_vertex::~scene_vertex()
{
	glDeleteProgram(shader_program);
}

void scene_vertex::compile_shaders() {
	shader_program = generateShaderProgram({
		{ "shaders/grid.vert", GL_VERTEX_SHADER},
		{ "shaders/solid_color.frag", GL_FRAGMENT_SHADER },
		});
}

void scene_vertex::init()
{
	compile_shaders();
}

void scene_vertex::awake()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_PROGRAM_POINT_SIZE);
}

void scene_vertex::sleep()
{
	glClearColor(1.0f, 1.0f, 0.5f, 1.0f);
	glDisable(GL_PROGRAM_POINT_SIZE);
}

void scene_vertex::mainLoop()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(shader_program);

	GLuint time_location = glGetUniformLocation(shader_program, "u_time");
	glUniform1f(time_location, time::elapsed_time().count());

	glDrawArrays(GL_POINTS, 0, 100);
	glUseProgram(0);
}

void scene_vertex::resize(int width, int height)
{
}

void scene_vertex::normalKeysDown(unsigned char key)
{
	if (key == 'c') {
		compile_shaders();
	}
}