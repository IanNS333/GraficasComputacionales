#pragma once

#include "scenes/base/scene_basic.h"
#include "vec2.h"

#include <vector>

class scene_chaikin : public scene_basic {
  public:
    void init();
    void awake();
    void sleep() {}
    void reset();
    void mainLoop();
    void keysDown(int key);
    void keysUp(int key) {}
    void passiveMotion(float x, float y) {}

  private:
    const float dashed_line_length = 0.02f;
    int chaikin_steps = 4;
    int makeDashedLine(std::vector<cgmath::vec2>& buffer, cgmath::vec2 b);
    std::vector<cgmath::vec2> chaikin(std::vector<cgmath::vec2>& poligon,
                                      int steps);
    GLuint p_vao, p_vbo, c_vao, c_vbo;
    std::vector<int> p_starts, c_starts, p_sizes, c_sizes;
    std::vector<std::vector<cgmath::vec2>> poligons = {
        // left ear
        {{-0.29f, 0.24f},
         {-0.34f, 0.29f},
         {-0.52f, 0.43f},
         {-0.75f, 0.54f},
         {-0.91f, 0.54f}},
        {{-0.75f, 0.38f}, {-0.67f, 0.45f}, {-0.58f, 0.46f}},
        {{-0.37f, 0.12f}, {-0.59f, 0.24f}, {-0.86f, 0.48f}, {-0.91f, 0.54f}},
        // right ear
        {{0.27f, 0.23f}, {0.42f, 0.29f}, {0.73f, 0.34f}, {0.91f, 0.34f}},
        {{0.66f, 0.18f}, {0.69f, 0.24f}, {0.69f, 0.33f}},
        {{0.32f, 0.10f}, {0.45f, 0.11f}, {0.74f, 0.20f}, {0.91f, 0.34f}},
        // face contour
        {{0.32f, 0.10f},
         {0.34f, 0.08f},
         {0.46f, -0.40f},
         {0.04f, -0.53f},
         {-0.02f, -0.55f},
         {-0.53f, -0.41f},
         {-0.37f, 0.09f},
         {-0.37f, 0.12f}},
        {{-0.29f, 0.24f}, {-0.02f, 0.29f}, {0.27f, 0.23f}},
        // cheeks
        {{0.40f, -0.17f},
         {0.37f, -0.15f},
         {0.29f, -0.21f},
         {0.25f, -0.31f},
         {0.29f, -0.41f},
         {0.37f, -0.39f}},
        {{-0.45f, -0.17f},
         {-0.42f, -0.15f},
         {-0.34f, -0.21f},
         {-0.30f, -0.31f},
         {-0.36f, -0.41f},
         {-0.43f, -0.39f}},
        // eyes
        {{-0.22f, -0.04f},
         {-0.19f, -0.09f},
         {-0.19f, -0.14f},
         {-0.22f, -0.19f},
         {-0.29f, -0.19f},
         {-0.32f, -0.14f},
         {-0.32f, -0.09f},
         {-0.29f, -0.04f},
         {-0.22f, -0.04f}},
        {{-0.225f, -0.048f},
         {-0.205f, -0.08f},
         {-0.205f, -0.104f},
         {-0.225f, -0.136f},
         {-0.265f, -0.136f},
         {-0.285f, -0.104f},
         {-0.285f, -0.08f},
         {-0.265f, -0.048f},
         {-0.225f, -0.048f}},
        {{0.17f, -0.04f},
         {0.14f, -0.09f},
         {0.14f, -0.14f},
         {0.17f, -0.19f},
         {0.24f, -0.19f},
         {0.27f, -0.14f},
         {0.27f, -0.09f},
         {0.24f, -0.04f},
         {0.17f, -0.04f}},
        {{0.175f, -0.048f},
         {0.155f, -0.08f},
         {0.155f, -0.104f},
         {0.175f, -0.136f},
         {0.215f, -0.136f},
         {0.235f, -0.104f},
         {0.235f, -0.08f},
         {0.215f, -0.048f},
         {0.175f, -0.048f}},
        // mouth
        {{-0.16f, -0.28f}, {-0.11f, -0.33f}, {-0.03f, -0.29f}},
        {{-0.03f, -0.29f}, {0.07f, -0.32f}, {0.10f, -0.28f}},
        // nose
        {{-0.01f, -0.198f},
         {-0.003f, -0.205f},
         {-0.018f, -0.218f},
         {-0.036f, -0.218f},
         {-0.052f, -0.205f},
         {-0.045f, -0.198f},
         {-0.01f, -0.198f}}};
    bool show_poligons = true, show_curves = true;
};
