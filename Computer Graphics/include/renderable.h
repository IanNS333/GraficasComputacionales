#pragma once

#include <glad/glad.h>
#include <stdexcept>
#include "mat3.h"
#include "mat4.h"
#include "vec2.h"
#include "vec3.h"
#include "vec4.h"

template <typename T>
struct uniform_variable {
    uniform_variable() {}
    uniform_variable(GLint location, const T& value)
        : location(location), value(value) {}
    inline void setValue(const T& val) {
        value = val;
        dirty = 1;
    }
    inline void send() {
        if (!dirty) {
            return;
        }
        sendValue();
        dirty = 0;
    };
    inline void send(const T& val) {
        setValue(val);
        sendValue();
        dirty = 0;
    }

  private:
    GLint location;
    T value;
    bool dirty = 1;
    inline void sendValue() {
        throw std::runtime_error("send not implemented for type");
    }
};

// windows needs the specializations to be in the h file
// clang needs them on the cpp file
#ifdef _WIN32

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
