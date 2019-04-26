#pragma once

#include <IL/il.h>
#include <initializer_list>
#include <string>
#include <vector>
#include "scenes/base/scene.h"
#include "vec2.h"

struct screen {
    static int width, height;
    static GLFWwindow* window;
};

constexpr const float PI = 3.14159265358979323846f;

constexpr float rad(int x) {
    return x * PI / 180.0f;
}

GLuint loadShaderFromFile(const std::string& filename, GLenum shader_type);

GLuint createShaderProgram(
    std::initializer_list<std::pair<std::string, GLenum>> shader_info,
    std::initializer_list<std::string> variables = {});

template <class T>
void addBuffer(GLuint vao,
               GLuint& vbo,
               const std::vector<T>& data,
               GLuint idx = 0,
               GLuint divisor = 0) {
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(T), data.data(),
                 GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(idx);
    glVertexAttribPointer(idx, sizeof(T) / sizeof(float), GL_FLOAT, GL_FALSE, 0,
                          nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    if (divisor) {
        glVertexAttribDivisor(idx, divisor);
    }
    glBindVertexArray(0);
}

void addIndexBuffer(GLuint vao,
                    GLuint& vbo,
                    const std::vector<unsigned int>& indices);

void addTexture(const std::string& filename, GLuint& texture);
