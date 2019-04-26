#pragma once

#include "scenes/base/scene.h"
#include "_util.h"

class scene_basic : public scene {
  public:
    virtual void init() = 0;
    virtual void awake();
    virtual void sleep();
    virtual void reset() = 0;
    virtual void mainLoop() = 0;
    virtual void resize(int width, int height);
    virtual void keysDown(int key) = 0;
    virtual void keysUp(int key) = 0;
    virtual void passiveMotion(float x, float y) = 0;

    virtual ~scene_basic() {}
};
