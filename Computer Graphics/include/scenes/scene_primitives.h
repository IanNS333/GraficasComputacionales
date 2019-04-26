#pragma once

#include "scenes/base/scene_basic.h"

class scene_primitives : public scene_basic {
  public:
    void init();
    void awake();
    void sleep();
    void reset() {}
    void mainLoop();
    void keysDown(int key);
    void keysUp(int key) {}
    void passiveMotion(float x, float y) {}

  private:
    GLuint vao;
    GLuint positionsVBO;

    GLenum primitiveType;
};
