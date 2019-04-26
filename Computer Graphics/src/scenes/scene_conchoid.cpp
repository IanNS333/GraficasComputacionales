#include "scenes/scene_conchoid.h"

#include "_util.h"
#include <cmath>

void scene_conchoid::init() {
    std::vector<cgmath::vec2> positions;
    // top line
    for (float t = -0.55f; t < 0.56f; t += 0.05f) {
        positions.emplace_back(t + k * t / sqrt(b * b + t * t),
                               b + b * k / sqrt(b * b + t * t));
    }
    t_vCount = positions.size();
    glGenVertexArrays(1, &t_vao);
    addBuffer(t_vao, t_vbo, positions);

    // bottom line
    positions.clear();
    for (float t = -1.5; t < 1.5f; t += 0.05f) {
        positions.emplace_back(t - k * t / sqrt(b * b + t * t),
                               b + b * -k / sqrt(b * b + t * t));
    }
    b_vCount = positions.size();
    glGenVertexArrays(1, &b_vao);
    addBuffer(b_vao, b_vbo, positions);
}

void scene_conchoid::awake() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void scene_conchoid::mainLoop() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindVertexArray(b_vao);
    glDrawArrays(GL_LINE_STRIP, 0, b_vCount);

    glBindVertexArray(t_vao);
    glDrawArrays(GL_LINE_STRIP, 0, t_vCount);

    glBindVertexArray(0);
}
