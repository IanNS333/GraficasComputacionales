#pragma once

#include "scenes/base/scene_shader.h"
#include "_util.h"

class scene_fragment : public scene_shader {
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
    GLuint time_location, resolution_location;
    GLuint vao, positionsBuffer, indicesBuffer;
    void sendResolution();
};
