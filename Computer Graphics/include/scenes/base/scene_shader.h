#pragma once

#include "scenes/base/scene_basic.h"

class scene_shader : public scene_basic {
  public:
    virtual void init() = 0;
    virtual void reset() = 0;
    virtual void mainLoop() = 0;
    virtual void keysDown(int key) = 0;
    virtual void keysUp(int key) = 0;
    virtual void passiveMotion(float x, float y) = 0;

    virtual ~scene_shader();

  protected:
    GLuint shader_program;
};
