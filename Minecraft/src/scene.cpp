#include <vector>
#include <string>
#include <iostream>

#include "scene.h"

#include "ifile.h"

GLuint scene::generateShaderProgram(std::vector<shader_file> shaders, std::vector<std::string> variables) {
	
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

	int index = 0;
	for (auto variable : variables) {
		glBindAttribLocation(shader_program, index, variable.c_str());
		index++;
	}

	// Deberiamos de verificar que el proceso de linkeo termine sin errores.
	glLinkProgram(shader_program);

	for (auto shader_id : shaderIds) {
		glDeleteShader(shader_id);
	}

	return shader_program;
}

GLuint scene::generateTexture(std::string path, GLenum min_filter, GLenum mag_filter, GLenum wrap_s, GLenum wrap_t) {
	ILuint imageId;
	GLuint textureId;

	ilGenImages(1, &imageId);
	ilBindImage(imageId);
	ilLoadImage(path.c_str());

	glGenTextures(1, &textureId);
	//glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);

	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		ilGetInteger(IL_IMAGE_FORMAT),
		ilGetInteger(IL_IMAGE_WIDTH),
		ilGetInteger(IL_IMAGE_HEIGHT),
		0,
		ilGetInteger(IL_IMAGE_FORMAT),
		ilGetInteger(IL_IMAGE_TYPE),
		ilGetData()
	);

	glBindTexture(GL_TEXTURE_2D, 0);
	
	ilBindImage(0);
	ilDeleteImages(1, &imageId);

	return textureId;
}