#pragma once

#include "scenes/base/scene_mvp.h"

#include <vector>
#include "mat4.h"

class scene_texture : public scene_mvp {
  public:
    void init();
    void reset() {}
    void mainLoop();
    void keysDown(int key) {}
    void keysUp(int key) {}
    void passiveMotion(float x, float y) {}

    ~scene_texture();

  private:
    GLuint positionsBuffer, textureBuffer, indicesBuffer, n_points = 6 * 6;
    GLuint crate_texture, pig_texture;
    GLuint floorShader, floor_vao, floor_pos, floor_texture, floorTextureBuffer;
};
