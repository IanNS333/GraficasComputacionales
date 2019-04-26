#pragma once

#include "scenes/base/scene_shader.h"

#include <vector>

class scene_circle : public scene_shader {
  public:
    void init();
    void awake();
    void reset() {}
    void mainLoop();
    void resize(int width, int height);
    void keysDown(int key) {}
    void keysUp(int key) {}
    void passiveMotion(float x, float y) {}

  private:
    GLuint vao, positionsBuffer, colorsBuffer, n_points, resolution_location;
    void sendResolution();
};
