#pragma once

#include "scenes/base/scene_basic.h"
#include "vec2.h"

#include <vector>

class scene_conchoid : public scene_basic {
  public:
    void init();
    void awake();
    void sleep() {}
    void reset() {}
    void mainLoop();
    void keysDown(int key) {}
    void keysUp(int key) {}
    void passiveMotion(float x, float y) {}

  private:
    float b = 0.25f, k = 0.5f;
    int b_vCount, t_vCount;

    GLuint b_vao, t_vao;
    GLuint b_vbo, t_vbo;
};
