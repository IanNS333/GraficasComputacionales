#pragma once

#include "scenes/base/scene_mvp.h"

#include <glad/glad.h>
#include <vector>
#include "camera.h"
#include "mat4.h"
#include "renderable.h"

class scene_final : public scene_mvp {
  public:
    void init();
    void awake();
    void sleep();
    void reset() {}
    void mainLoop();
    void keysDown(int key);
    void keysUp(int key);
    void passiveMotion(float x, float y);
    void resize(int width, int height);

    ~scene_final() {}

  private:
    uniform_variable<cgmath::mat4> vp;
    uniform_variable<cgmath::vec3> lightColor, lightPosition;
    GLuint positions, normals, indices, texPos, offsets, texOffsets, texture;
    Camera camera;
};
