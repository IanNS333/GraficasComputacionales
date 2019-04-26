#include "scenes/scene_circle.h"

#include <cmath>
#include "_util.h"
#include "vec2.h"
#include "vec3.h"

void scene_circle::init() {
    std::vector<cgmath::vec2> positions;
    positions.emplace_back(0.0f, 0.0f);
    std::vector<cgmath::vec3> colors;
    colors.emplace_back(1.0f, 1.0f, 1.0f);

    for (int i = 0; i < 360; ++i) {
        positions.emplace_back(cos(rad(i)), sin(rad(i)));
        colors.emplace_back(sin(rad(i)) / 2 + 0.5f, cos(rad(i)) / 2 + 0.5f,
                            sin(rad(i) - PI / 2) / 2 + 0.5f);
    }

    positions.push_back(positions[1]);
    colors.push_back(colors[1]);

    n_points = positions.size();

    glGenVertexArrays(1, &vao);
    addBuffer(vao, positionsBuffer, positions);
    addBuffer(vao, colorsBuffer, colors, 1);

    shader_program =
        createShaderProgram({{"shaders/simple_color.vert", GL_VERTEX_SHADER},
                             {"shaders/circle.frag", GL_FRAGMENT_SHADER}},
                            {"position", "color"});

    resolution_location = glGetUniformLocation(shader_program, "resolution");
}

void scene_circle::awake() {
    scene_shader::awake();
    sendResolution();
}

void scene_circle::mainLoop() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shader_program);
    glBindVertexArray(vao);

    glDrawArrays(GL_TRIANGLE_FAN, 0, n_points);

    glBindVertexArray(0);
    glUseProgram(0);
}

void scene_circle::resize(int width, int height) {
    scene_shader::resize(width, height);
    sendResolution();
}

void scene_circle::sendResolution() {
    glUseProgram(shader_program);
    glUniform2f(resolution_location, screen::width, screen::height);
    glUseProgram(0);
}
