#pragma once

#include "scenes/base/scene_basic.h"

class scene_sphere : public scene_basic {
  public:
    void init();
    void awake();
    void sleep();
    void reset();
    void mainLoop();
    void keysDown(int key) {}
    void keysUp(int key) {}
    void passiveMotion(float x, float y) {}

    ~scene_sphere();

  private:
    GLuint shader_program;
    float angle = 0;
};
