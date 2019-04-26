#include "scenes/scene_circle_grid.h"

#include "_time.h"
#include "_util.h"

void scene_circle_grid::init() {
    shader_program =
        createShaderProgram({{"shaders/circle_grid.vert", GL_VERTEX_SHADER},
                             {"shaders/solid_color.frag", GL_FRAGMENT_SHADER}});
    time_location = glGetUniformLocation(shader_program, "time");
}

void scene_circle_grid::mainLoop() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shader_program);
    glBindVertexArray(1);
    glUniform1f(time_location, time::elapsed_time().count());

    for (int i = 0; i < circles; ++i) {
        glDrawArrays(GL_TRIANGLE_STRIP, i * 42, 42);
    }

    glBindVertexArray(0);
    glUseProgram(0);
}

scene_circle_grid::~scene_circle_grid() {
    glDeleteProgram(shader_program);
}
