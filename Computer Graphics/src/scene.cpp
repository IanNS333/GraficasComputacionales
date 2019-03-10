#include <vector>
#include <string>
#include <iostream>

#include "scene.h"

#include "ifile.h"

GLuint scene::generateShaderProgram(std::vector<shader_file> shaders) {
	
	ifile file;

	GLuint shader_program = glCreateProgram();
	std::vector<GLuint> shaderIds;

	for (auto shader : shaders) {
		file.read(shader.path);
		std::string shader_source = file.get_contents();

		const GLchar* shader_source_c = (const GLchar *)shader_source.c_str();

		GLuint shader_id = glCreateShader(shader.type);

		glShaderSource(shader_id, 1, &shader_source_c, nullptr);
		glCompileShader(shader_id);

		GLint shader_compiled;
		glGetShaderiv(shader_id, GL_COMPILE_STATUS, &shader_compiled);
		if (shader_compiled != GL_TRUE) {
			GLint log_length;
			glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_length);

			std::vector<GLchar> log;
			log.resize(log_length);

			glGetShaderInfoLog(shader_id, log_length, &log_length, &log[0]);

			std::cout << log.data();
		}

		glAttachShader(shader_program, shader_id);
		shaderIds.push_back(shader_id);
	}

	// Deberiamos de verificar que el proceso de linkeo termine sin errores.
	glLinkProgram(shader_program);

	for (auto shader_id : shaderIds) {
		glDeleteShader(shader_id);
	}

	return shader_program;
}