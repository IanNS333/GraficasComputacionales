#include "scene_fragment.h"

#include "time_utils.h"
#include "vec2.h"

scene_fragment::~scene_fragment()
{
	glDeleteProgram(shader_program);
}

void scene_fragment::compile_shaders() {
	shader_program = generateShaderProgram({
		{"shaders/simple.vert", GL_VERTEX_SHADER},
		{"shaders/palmera.frag", GL_FRAGMENT_SHADER },
	},{
		"position"
	});

	time_location = glGetUniformLocation(shader_program, "u_time");
	resolution_location = glGetUniformLocation(shader_program, "u_resolution");

	glUseProgram(shader_program);
	glUniform2f(resolution_location, 400, 400);
	glUseProgram(0);
}

void scene_fragment::init()
{

	std::vector<cgmath::vec2> positions;
	positions.push_back(cgmath::vec2(-1.0f, -1.0f));
	positions.push_back(cgmath::vec2(1.0f, -1.0f));
	positions.push_back(cgmath::vec2(1.0f, 1.0f));
	positions.push_back(cgmath::vec2(-1.0f, 1.0f));

	std::vector<unsigned int> indices{ 0, 1, 3, 1, 2, 3};

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	positionsVBO = generateVBO(vao, 0, positions, 2, GL_FLOAT, GL_STATIC_DRAW);
	indicesBuffer= generateIBO(vao, indices, GL_STATIC_DRAW);

	glBindVertexArray(0);

	compile_shaders();

	//GLuint positionAttributeLocation = glGetAttribLocation(shader_program, "position");
}

void scene_fragment::awake()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_PROGRAM_POINT_SIZE);
}

void scene_fragment::sleep()
{
	glClearColor(1.0f, 1.0f, 0.5f, 1.0f);
	glDisable(GL_PROGRAM_POINT_SIZE);
}

void scene_fragment::mainLoop()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBuffer);

	glUseProgram(shader_program);

	glUniform1f(time_location, time::elapsed_time().count());

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glUseProgram(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void scene_fragment::resize(int width, int height)
{
	glViewport(0, 0, width, height);

	glUseProgram(shader_program);

	glUniform2f(resolution_location, (float)width, (float)height);

	glUseProgram(0);
	//this->width = width;
	//this->height = height;
}

void scene_fragment::normalKeysDown(unsigned char key)
{
	if (key == 'c') {
		compile_shaders();
	}
}