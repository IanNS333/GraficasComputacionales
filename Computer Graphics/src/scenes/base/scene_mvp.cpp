#include "scenes/base/scene_mvp.h"

#include "_time.h"
#include "vec3.h"

void scene_mvp::resize(int width, int height) {
    scene_shader::resize(width, height);
    projection = cgmath::mat4::perspective(
        float(screen::width) / screen::height, fov, _near, _far);
}
