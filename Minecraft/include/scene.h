#pragma once

#include <glad/glad.h>

#define GLFW_INCLUDE_NONE
#define GLFW_DLL
#include <GLFW/glfw3.h>

#include <IL/il.h>
#include <string>
#include <vector>
#include <thread>
#include <atomic>

struct shader_file {
    shader_file(std::string path, GLenum type) : path(path), type(type) {}
    std::string path;
    GLenum type;
};

// Scene is an abstract class.
class scene {
  protected:
    GLFWwindow* window;
  public:
    scene(GLFWwindow *window): window(window) {
	}

    virtual ~scene() {}

    virtual void init() = 0;
    virtual void awake() = 0;
    virtual void sleep() = 0;
    virtual void reset() = 0;
    virtual void mainLoop() = 0;
    virtual void resize(int width, int height) = 0;
    virtual void keysDown(int key) = 0;
    virtual void keysUp(int key) = 0;
    virtual void passiveMotion(float x, float y) = 0;
	virtual void mouseButton(int button, int action) = 0;

    GLuint generateShaderProgram(std::vector<shader_file> shaders,
                                 std::vector<std::string> variables = {});
    GLuint generateTexture(std::string path,
                           GLenum min_filter = GL_NEAREST,
                           GLenum mag_filter = GL_NEAREST,
                           GLenum wrap_s = GL_REPEAT,
                           GLenum wrap_t = GL_REPEAT);

    template <class T>
    GLuint generateVBO(const GLuint& vao,
                       const GLuint& attribId,
                       const std::vector<T>& data,
                       const GLuint& dataTypeCount,
                       const GLenum& dataType,
                       const GLenum& drawType) {
        GLuint result;
        glBindVertexArray(vao);

        glGenBuffers(1, &result);
        glBindBuffer(GL_ARRAY_BUFFER, result);
        glBufferData(GL_ARRAY_BUFFER, sizeof(T) * data.size(), data.data(),
                     drawType);
        glEnableVertexAttribArray(attribId);
        glVertexAttribPointer(attribId, dataTypeCount, dataType, GL_FALSE, 0,
                              nullptr);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        return result;
    }

    template <class T>
    GLuint generateInstancedVBO(const GLuint& vao,
                                const GLuint& attribId,
                                const std::vector<T>& data,
                                const GLuint& dataTypeCount,
                                const GLenum& dataType,
                                const GLenum& drawType,
                                const GLuint& instances_pass_per_value) {
        GLuint result;
        glBindVertexArray(vao);

        glGenBuffers(1, &result);
        glBindBuffer(GL_ARRAY_BUFFER, result);
        glBufferData(GL_ARRAY_BUFFER, sizeof(T) * data.size(), data.data(),
                     drawType);
        glEnableVertexAttribArray(attribId);
        glVertexAttribPointer(attribId, dataTypeCount, dataType, GL_FALSE, 0,
                              nullptr);
        glVertexAttribDivisor(attribId, instances_pass_per_value);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        return result;
    }

    template <class T>
    void updateVBO(const GLuint& vao,
                   const GLuint& vbo,
                   const GLuint& attribId,
                   const std::vector<T>& data,
                   const GLuint& dataTypeCount,
                   const GLenum& dataType,
                   const GLenum& drawType) {
        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(T) * data.size(), data.data(),
                     drawType);
        glEnableVertexAttribArray(attribId);
        glVertexAttribPointer(attribId, dataTypeCount, dataType, GL_FALSE, 0,
                              nullptr);

        // glBindBuffer(GL_ARRAY_BUFFER, 0);
        // glBindVertexArray(0);
    }

    template <class T>
    void updateInstancedVBO(const GLuint& vao,
                            const GLuint& vbo,
                            const GLuint& attribId,
                            const std::vector<T>& data,
                            const GLuint& dataTypeCount,
                            const GLenum& dataType,
                            const GLenum& drawType,
                            const GLuint& instances_pass_per_value) {
        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(T) * data.size(), data.data(),
                     drawType);
        glEnableVertexAttribArray(attribId);
        glVertexAttribPointer(attribId, dataTypeCount, dataType, GL_FALSE, 0,
                              nullptr);
        glVertexAttribDivisor(attribId, instances_pass_per_value);

        // glBindBuffer(GL_ARRAY_BUFFER, 0);
        // glBindVertexArray(0);
    }

    GLuint generateIBO(const GLuint& vao,
                       const std::vector<unsigned int>& indexes,
                       const GLenum& drawType) {
        GLuint result;
        glBindVertexArray(vao);

        glGenBuffers(1, &result);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, result);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     sizeof(unsigned int) * indexes.size(), indexes.data(),
                     drawType);

        glBindVertexArray(vao);
        return result;
    }
};