#include "_util.h"

#include "ifile.h"

GLuint loadShaderFromFile(const std::string& filename, GLenum shader_type) {
    ifile shader_file;

    shader_file.read(filename);
    std::string source = shader_file.get_contents();
    const GLchar* source_c = (const GLchar*)source.c_str();
    GLuint shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &source_c, nullptr);
    glCompileShader(shader);

    GLint compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (compiled != GL_TRUE) {
        GLint log_length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);

        std::vector<GLchar> log(log_length);
        glGetShaderInfoLog(shader, log_length, &log_length, log.data());
        std::cout << std::endl << filename << ":\n" << log.data();

        return 0;
    }

    return shader;
}

GLuint createShaderProgram(
    std::initializer_list<std::pair<std::string, GLenum>> shader_info,
    std::initializer_list<std::string> variables) {
    std::vector<GLuint> v;
    for (auto& p : shader_info) {
        v.push_back(loadShaderFromFile(p.first, p.second));
    }

    GLuint shader_program = glCreateProgram();
    for (auto& shader : v) {
        glAttachShader(shader_program, shader);
    }

    {
        int i = 0;
        for (auto& variable : variables) {
            glBindAttribLocation(shader_program, i++, variable.c_str());
        }
    }

    glLinkProgram(shader_program);

    GLint linked;
    glGetProgramiv(shader_program, GL_LINK_STATUS, &linked);
    if (linked != GL_TRUE) {
        GLint log_length;
        glGetProgramiv(shader_program, GL_INFO_LOG_LENGTH, &log_length);

        std::vector<GLchar> log(log_length);
        glGetProgramInfoLog(shader_program, log_length, &log_length,
                            log.data());
        std::cout << std::endl << log.data();
    }

    for (auto shader : v) {
        glDeleteShader(shader);
    }

    return shader_program;
}

void addIndexBuffer(GLuint vao,
                    GLuint& vbo,
                    const std::vector<unsigned int>& indices) {
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(),
                 indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void addTexture(const std::string& filename, GLuint& texture) {
    ILuint image;
    ilGenImages(1, &image);
    ilBindImage(image);
    ilLoadImage(filename.c_str());

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // GL_NEAREST_MIPMAP_NEAREST: Uses the mipmap that most closely matches the
    // size of the pixel being textured and samples with nearest neighbour
    // interpolation.

    // GL_LINEAR_MIPMAP_NEAREST: Samples the closest mipmap with linear
    // interpolation.

    // GL_NEAREST_MIPMAP_LINEAR: Uses the two mipmaps that most closely match
    // the size of the pixel being textured and samples with nearest neighbour
    // interpolation.

    // GL_LINEAR_MIPMAP_LINEAR: Samples closest two mipmaps with linear
    // interpolation.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    ILint format = ilGetInteger(IL_IMAGE_FORMAT);
    glTexImage2D(GL_TEXTURE_2D, 0, format, ilGetInteger(IL_IMAGE_WIDTH),
                 ilGetInteger(IL_IMAGE_HEIGHT), 0, format,
                 ilGetInteger(IL_IMAGE_TYPE), ilGetData());

    glGenerateMipmap(GL_TEXTURE_2D);

    ilBindImage(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    ilDeleteImages(1, &image);
}

int screen::width = 800;
int screen::height = 800;
GLFWwindow* screen::window = nullptr;
