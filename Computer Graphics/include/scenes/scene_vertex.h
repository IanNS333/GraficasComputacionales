#pragma once

#include "scenes/base/scene_basic.h"

#include <string>

class scene_vertex : public scene_basic {
  public:
    void init();
    void awake();
    void sleep();
    void reset() {}
    void mainLoop();
    void keysDown(int key) {}
    void keysUp(int key) {}
    void passiveMotion(float x, float y) {}

    ~scene_vertex();

  private:
    GLuint shader_program;
};
