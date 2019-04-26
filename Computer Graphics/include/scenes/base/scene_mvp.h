#pragma once

#include "scenes/base/scene_shader.h"

#include "mat4.h"

class scene_mvp : public scene_shader {
  public:
    virtual void init() = 0;
    virtual void reset() {}
    virtual void mainLoop() = 0;
    virtual void resize(int width, int height);
    virtual void keysDown(int key) {}
    virtual void keysUp(int key) {}
    virtual void passiveMotion(float x, float y) {}

  protected:
    float fov, _near, _far;
    GLuint vao, mvp_location;
    cgmath::mat4 model, view, projection;
};
