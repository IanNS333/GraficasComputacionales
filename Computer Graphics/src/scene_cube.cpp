#include "scene_cube.h"

#include "time_utils.h"
#include "vec3.h"
#include "vec4.h"
#include "mat4.h"
#include "cgmath.h"

using cgmath::vec3;
using cgmath::vec4;
using cgmath::mat4;
using cgmath::radians;

scene_cube::~scene_cube()
{
	glDeleteProgram(shader_program);
}

void scene_cube::compile_shaders() {
	shader_program = generateShaderProgram({
		{"shaders/cube.vert", GL_VERTEX_SHADER},
		{"shaders/cube.frag", GL_FRAGMENT_SHADER },
		}, {
			"position",
			"color",
		});

	time_location = glGetUniformLocation(shader_program, "u_time");
	transformation_matrix_location = glGetUniformLocation(shader_program, "u_transformationMatrix");

	glUseProgram(shader_program);
	glUseProgram(0);
}

cgmath::mat4 scene_cube::model_matrix(float time) {

	float deltaX = 30.0f;
	float deltaY = 60.0f;
	float deltaZ = 30.0f;

	float alpha = radians(time * deltaX);
	float beta = radians(time * deltaY);
	float gamma = radians(time * deltaZ);

	mat4 rotationX = mat4::transpose({
		{1.0f, 0.0f, 0.0f, 0.0f},
		{0.0f, cos(alpha), -sin(alpha), 0.0f},
		{0.0f, sin(alpha), cos(alpha), 0.0f},
		{0.0f, 0.0f, 0.0f, 1.0f}
	});
	mat4 rotationY = mat4::transpose({
		{cos(beta), 0.0f, sin(beta), 0.0f},
		{0.0f, 1.0f, 0.0f, 0.0f},
		{-sin(beta), 0.0f, cos(beta), 0.0f},
		{0.0f, 0.0f, 0.0f, 1.0f}
	});
	mat4 rotationZ = mat4::transpose({
		{cos(gamma), -sin(gamma), 0.0f, 0.0f},
		{sin(gamma), cos(gamma), 0.0f, 0.0f},
		{0.0f, 0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 0.0f, 1.0f}
	});

	mat4 scale = {
		{ 3.0f, 0.0f, 0.0f, 0.0f},
		{ 0.0f, 3.0f, 0.0f, 0.0f},
		{ 0.0f, 0.0f, 3.0f, 0.0f},
		{ 0.0f, 0.0f, 0.0f, 1.0f},
	};

	return rotationZ * rotationY * rotationX * scale;
}

void scene_cube::init()
{

	cgmath::vec3 back_lower_left = { -1.0f, -1.0f, -1.0f };
	cgmath::vec3 back_lower_right = { 1.0f, -1.0f, -1.0f };
	cgmath::vec3 back_upper_right = { 1.0f, 1.0f, -1.0f };
	cgmath::vec3 back_upper_left = { -1.0f, 1.0f, -1.0f };
	cgmath::vec3 front_lower_left = { -1.0f, -1.0f, 1.0f };
	cgmath::vec3 front_lower_right = { 1.0f, -1.0f, 1.0f };
	cgmath::vec3 front_upper_right = { 1.0f, 1.0f, 1.0f };
	cgmath::vec3 front_upper_left = { -1.0f, 1.0f, 1.0f };

	std::vector<cgmath::vec3> positions = {
		//front
		front_lower_left, front_lower_right, front_upper_right, front_upper_left,
		//left
		back_lower_left, front_lower_left, front_upper_left, back_upper_left,
		//right
		front_lower_right, back_lower_right, back_upper_right, front_upper_right,
		//up
		front_upper_left, front_upper_right, back_upper_right, back_upper_left,
		//down
		back_lower_left, back_lower_right, front_lower_right, front_lower_left,
		//back
		back_upper_left, back_upper_right, back_lower_right, back_lower_left,
	};

	std::vector<unsigned int> indices{
		// front
		0, 1, 2, 0, 2, 3,
		// left
		4, 5, 6, 4, 6, 7,
		// right
		8, 9, 10, 8, 10, 11,
		// up
		12, 13, 14, 12, 14, 15,
		// down
		16, 17, 18, 16, 18, 19,
		// back
		20, 21, 22, 20, 22, 23,
	};

	cgmath::vec3 blue = cgmath::vec3({ 22.0f, 100.0f, 226.0f }) / 255.0f;
	cgmath::vec3 red = cgmath::vec3({ 193.0f, 26.0f, 7.0f }) / 255.0f;
	cgmath::vec3 green = cgmath::vec3({ 21.0f, 153.0f, 4.0f }) / 255.0f;
	cgmath::vec3 orange = cgmath::vec3({ 229.0f, 138.0f, 2.0f }) / 255.0f;
	cgmath::vec3 aqua = cgmath::vec3({ 2.0f, 181.0f, 252.0f }) / 255.0f;
	cgmath::vec3 purple = cgmath::vec3({ 144.0f, 79.0f, 214.0f }) / 255.0f;

	std::vector<cgmath::vec3> colors = {
		blue, blue, blue, blue,
		red, red, red, red,
		green, green, green, green,
		orange, orange, orange, orange,
		aqua, aqua, aqua, aqua,
		purple, purple, purple, purple,
	};

	glGenVertexArrays(1, &vao);

	indexVBO = generateIBO(vao, indices, GL_STATIC_DRAW);
	positionsVBO = generateVBO(vao, 0, positions, 3, GL_FLOAT, GL_STATIC_DRAW);
	colorsVBO = generateVBO(vao, 1, colors, 3, GL_FLOAT, GL_STATIC_DRAW);

	compile_shaders();
}

void scene_cube::awake()
{
	glClearColor(208.0f / 255.0f, 183.0f / 255.0f, 249.0f / 255.0f, 1.0f);
}

void scene_cube::sleep()
{
	glClearColor(1.0f, 1.0f, 0.5f, 1.0f);
}

void scene_cube::mainLoop()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(vao);
	glUseProgram(shader_program);

	float u_time = time::elapsed_time().count();
	glUniform1f(time_location, u_time);

	mat4 transformation_matrix = proyection_matrix * mat4::inverse(view_matrix)*model_matrix(u_time);
	glUniformMatrix4fv(transformation_matrix_location, 1, GL_FALSE, &(transformation_matrix[0][0]));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

	glUseProgram(0);
	glBindVertexArray(0);
}

void scene_cube::resize(int width, int height)
{
	this->width = width;
	this->height = height;

	glViewport(0, 0, width, height);

	glUseProgram(shader_program);

	proyection_matrix = mat4::transpose({
		{1.0f / ((width + 0.0f)/(height + 0.0f) * tan(radians(60.0f) / 2.0f)), 0.0f, 0.0f, 0.0f},
		{0.0f, 1.0f / (tan(radians(60.0f) / 2.0f)), 0.0f, 0.0f},
		{0.0f, 0.0f, -(1000.0f + 1.0f) / (1000.0f - 1.0f), -(2.0f * 1000.0f * 1.0f) / (1000.0f - 1.0f)},
		{0.0f, 0.0f, -1.0f, 0.0f},
	});

	glUseProgram(0);
	//this->width = width;
	//this->height = height;
}

void scene_cube::normalKeysDown(unsigned char key)
{
	if (key == 'c') {
		compile_shaders();
	}
}