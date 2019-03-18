#define _USE_MATH_DEFINES

#include "scene_circle.h"

#include "time_utils.h"
#include "vec2.h"
#include "vec3.h"

#define TO_RADIANS (float)M_PI / 180.0f

scene_circle::~scene_circle()
{
	glDeleteProgram(shader_program);
}

void scene_circle::compile_shaders() {
	shader_program = generateShaderProgram({
		{"shaders/simple_color.vert", GL_VERTEX_SHADER},
		{"shaders/circle.frag", GL_FRAGMENT_SHADER },
		}, {
			"position",
			"color",
		});

	time_location = glGetUniformLocation(shader_program, "u_time");
	resolution_location = glGetUniformLocation(shader_program, "u_resolution");

	glUseProgram(shader_program);
	glUniform2f(resolution_location, (float)width, (float)height);
	glUseProgram(0);
}

void scene_circle::init()
{

	std::vector<cgmath::vec2> positions;
	std::vector<cgmath::vec3> colors;

	positions.push_back({ 0.0f, 0.0f });
	colors.push_back({ 1.0f, 1.0f, 1.0f });

	for (float i = 0; i <= 360; i++) {
		positions.push_back({ cosf(i * TO_RADIANS), sinf(i * TO_RADIANS) });
		colors.push_back({ sinf(i * TO_RADIANS), cosf(i * TO_RADIANS), sinf(i*TO_RADIANS - ((float)M_PI / 2.0f)) });
	}

	glGenVertexArrays(1, &vao);

	positionsVBO = generateVBO(vao, 0, positions, 2, GL_FLOAT, GL_STATIC_DRAW);
	colorsVBO = generateVBO(vao, 1, colors, 3, GL_FLOAT, GL_STATIC_DRAW);

	compile_shaders();
}

void scene_circle::awake()
{
	glClearColor(208.0f/255.0f, 183.0f/255.0f, 249.0f/255.0f, 1.0f);
}

void scene_circle::sleep()
{
	glClearColor(1.0f, 1.0f, 0.5f, 1.0f);
}

void scene_circle::mainLoop()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(vao);

	glUseProgram(shader_program);

	glUniform1f(time_location, time::elapsed_time().count());

	glDrawArrays(GL_TRIANGLE_FAN, 0, 362);

	glUseProgram(0);
	glBindVertexArray(0);
}

void scene_circle::resize(int width, int height)
{
	this->width = width;
	this->height = height;

	glViewport(0, 0, width, height);

	glUseProgram(shader_program);

	glUniform2f(resolution_location, (float)width, (float)height);

	glUseProgram(0);
	//this->width = width;
	//this->height = height;
}

void scene_circle::normalKeysDown(unsigned char key)
{
	if (key == 'c') {
		compile_shaders();
	}
}