#pragma once

#include "scenes/base/scene_mvp.h"

#include <vector>
#include "mat4.h"
#include "renderable.h"

#include "phong_cube.h"

class scene_phong : public scene_mvp {
  public:
    void init();
    void reset() {}
    void mainLoop();
    void keysDown(int key) {}
    void keysUp(int key) {}
    void passiveMotion(float x, float y) {}
    void resize(int width, int height);

    ~scene_phong();

  private:
    GLuint shader_program, floor_vao, floor_pos, floor_texture,
        floorTextureBuffer;

    GLint model_location, vp_location, lightColor_location,
        lightPosition_location, normMat_location, cameraPosition_location;

    std::unique_ptr<phong_cube> c;
};
