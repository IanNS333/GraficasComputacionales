#include "scenes/scene_primitives.h"

#include "vec2.h"

#include <vector>

void scene_primitives::init() {
    std::vector<cgmath::vec2> positions;
    positions.emplace_back(0.0f, 0.0f);
    positions.emplace_back(-1.0f, 0.0f);
    positions.emplace_back(0.0f, -0.4f);
    positions.emplace_back(1.0f, 0.0f);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &positionsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, positionsVBO);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(cgmath::vec2),
                 positions.data(), GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    primitiveType = GL_POINTS;
}

void scene_primitives::awake() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glPointSize(20.0f);
}

void scene_primitives::sleep() {
    glPointSize(1.0f);
}

void scene_primitives::mainLoop() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindVertexArray(vao);

    glDrawArrays(primitiveType, 0, 4);

    glBindVertexArray(0);
}

void scene_primitives::keysDown(int key) {
    switch (key) {
        case GLFW_KEY_1:
            primitiveType = GL_POINTS;
            break;
        case GLFW_KEY_2:
            primitiveType = GL_LINES;
            break;
        case GLFW_KEY_3:
            primitiveType = GL_LINE_STRIP;
            break;
        case GLFW_KEY_4:
            primitiveType = GL_LINE_LOOP;
            break;
        case GLFW_KEY_5:
            primitiveType = GL_TRIANGLES;
            break;
        case GLFW_KEY_6:
            primitiveType = GL_TRIANGLE_STRIP;
            break;
        case GLFW_KEY_7:
            primitiveType = GL_TRIANGLE_FAN;
            break;
        default:
            break;
    }
}
