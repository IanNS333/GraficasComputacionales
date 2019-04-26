#include "scenes/scene_fragment.h"

#include <iostream>
#include <vector>
#include "ifile.h"
#include "_time.h"
#include "_util.h"
#include "vec2.h"

void scene_fragment::init() {
    std::vector<cgmath::vec2> positions = {
        {-1.0f, -1.0f}, {1.0f, -1.0f}, {1.0f, 1.0f}, {-1.0f, 1.0f}};

    std::vector<unsigned int> indices{0, 1, 3, 1, 2, 3};

    glGenVertexArrays(1, &vao);

    addBuffer(vao, positionsBuffer, positions);
    addIndexBuffer(vao, indicesBuffer, indices);

    shader_program =
        createShaderProgram({{"shaders/simple.vert", GL_VERTEX_SHADER},
                             {"shaders/palm_tree.frag", GL_FRAGMENT_SHADER}},
                            {"position"});

    time_location = glGetUniformLocation(shader_program, "time");
    resolution_location = glGetUniformLocation(shader_program, "resolution");
}

void scene_fragment::awake() {
    scene_shader::awake();
    sendResolution();
}

void scene_fragment::mainLoop() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shader_program);
    glBindVertexArray(vao);

    glUniform1f(time_location, time::elapsed_time().count());

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

	glBindVertexArray(0);
    glUseProgram(0);
}

void scene_fragment::resize(int width, int height) {
    scene_shader::resize(width, height);
    sendResolution();
}

void scene_fragment::sendResolution() {
    glUseProgram(shader_program);
    glUniform2f(resolution_location, screen::width, screen::height);
    glUseProgram(0);
}
