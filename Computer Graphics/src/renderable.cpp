#include "renderable.h"

// windows needs the specializations to be in the h file
// clang needs them on the cpp file
#ifndef _WIN32

template <>
void uniform_variable<float>::sendValue() {
    glUniform1f(location, value);
}

template <>
void uniform_variable<cgmath::vec2>::sendValue() {
    glUniform2fv(location, 1, &value[0]);
}

template <>
void uniform_variable<cgmath::vec3>::sendValue() {
    glUniform3fv(location, 1, &value[0]);
}

template <>
void uniform_variable<cgmath::vec4>::sendValue() {
    glUniform4fv(location, 1, &value[0]);
}

template <>
void uniform_variable<cgmath::mat3>::sendValue() {
    glUniformMatrix3fv(location, 1, GL_FALSE, &value[0][0]);
}

template <>
void uniform_variable<cgmath::mat4>::sendValue() {
    glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
}

#endif
