#include "scenes/base/scene_shader.h"

scene_shader::~scene_shader() {
    glDeleteProgram(shader_program);
}
