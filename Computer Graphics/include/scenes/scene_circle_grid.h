#pragma once

#include "scenes/base/scene_basic.h"

class scene_circle_grid : public scene_basic {
  public:
    void init();
    void reset() {}
    void mainLoop();
    void keysDown(int key) {}
    void keysUp(int key) {}
    void passiveMotion(float x, float y) {}

    ~scene_circle_grid();

  private:
    GLuint shader_program, time_location;
    int circles = 13 * 13;
};
