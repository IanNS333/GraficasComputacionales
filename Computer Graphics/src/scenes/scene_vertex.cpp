#include "scenes/scene_vertex.h"

#include <iostream>
#include <vector>
#include "ifile.h"
#include "_time.h"
#include "_util.h"

void scene_vertex::init() {
    shader_program =
        createShaderProgram({{"shaders/grid.vert", GL_VERTEX_SHADER},
                             {"shaders/solid_color.frag", GL_FRAGMENT_SHADER}});
}

void scene_vertex::awake() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_PROGRAM_POINT_SIZE);
}

void scene_vertex::sleep() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glDisable(GL_PROGRAM_POINT_SIZE);
}

void scene_vertex::mainLoop() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shader_program);
    glBindVertexArray(1);

    GLuint time_location = glGetUniformLocation(shader_program, "time");
    glUniform1f(time_location, time::elapsed_time().count());

    glDrawArrays(GL_POINTS, 0, 100);

	glBindVertexArray(0);
    glUseProgram(0);
}

scene_vertex::~scene_vertex() {
    glDeleteProgram(shader_program);
}
