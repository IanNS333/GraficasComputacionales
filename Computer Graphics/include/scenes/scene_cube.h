#pragma once

#include "scenes/base/scene_mvp.h"

#include <vector>
#include "mat4.h"

class scene_cube : public scene_mvp {
  public:
    void init();
    void reset() {}
    void mainLoop();
    void keysDown(int key) {}
    void keysUp(int key) {}
    void passiveMotion(float x, float y) {}

  private:
    GLuint positionsBuffer, colorsBuffer, indicesBuffer, n_points = 6 * 6;
};
