#include "scenes/base/scene_basic.h"

void scene_basic::awake() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void scene_basic::sleep() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void scene_basic::resize(int width, int height) {
    screen::width = width;
    screen::height = height;
    glViewport(0, 0, width, height);
}
