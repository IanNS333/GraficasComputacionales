#include "scene_sphere.h"

#include "time_utils.h"

scene_sphere::~scene_sphere()
{
	glDeleteProgram(shader_program);
}


void scene_sphere::compile_shaders() {
	shader_program = generateShaderProgram({
	{ "shaders/sphere.vert", GL_VERTEX_SHADER},
	{ "shaders/solid_color.frag", GL_FRAGMENT_SHADER },
		});
}

void scene_sphere::init()
{
	compile_shaders();
}

void scene_sphere::awake()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_PROGRAM_POINT_SIZE);
}

void scene_sphere::sleep()
{
	glClearColor(1.0f, 1.0f, 0.5f, 1.0f);
	glDisable(GL_PROGRAM_POINT_SIZE);
}

void scene_sphere::mainLoop()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(shader_program);

	GLuint time_location = glGetUniformLocation(shader_program, "u_time");
	glUniform1f(time_location, time::elapsed_time().count());
	
	glDrawArrays(GL_POINTS, 0, 3600 * 4);

	glUseProgram(0);
}

void scene_sphere::resize(int width, int height)
{
}

void scene_sphere::normalKeysDown(unsigned char key)
{
	if (key == 'c') {
		compile_shaders();
	}
}