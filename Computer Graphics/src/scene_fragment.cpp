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
			});
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

	glGenBuffers(1, &positionsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, positionsVBO);
	glBufferData(GL_ARRAY_BUFFER,
		sizeof(cgmath::vec2) * positions.size(),
		positions.data(),
		GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &indicesBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBuffer);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		sizeof(unsigned int) * indices.size(),
		indices.data(),
		GL_STATIC_DRAW
	);

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

	GLuint time_location = glGetUniformLocation(shader_program, "u_time");
	GLuint resolution_location = glGetUniformLocation(shader_program, "u_resolution");

	glUniform1f(time_location, time::elapsed_time().count());
	glUniform2f(resolution_location, width, height);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glUseProgram(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void scene_fragment::resize(int width, int height)
{
	//this->width = width;
	//this->height = height;
}

void scene_fragment::normalKeysDown(unsigned char key)
{
	if (key == 'c') {
		compile_shaders();
	}
}