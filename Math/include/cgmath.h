#pragma once

#include <math.h>
#include <limits>

#include "vec3.h"
#include "mat4.h"

namespace cgmath {

bool AreEqual(float a, float b);
float radians(float degrees);

mat4 rotate_around(const float &angles, const vec3 &v);

mat4 translation_matrix(const float &x, const float &y, const float &z);
mat4 translation_matrix(const vec3 &v);
mat4 rotation_matrix(const vec3 &v);
mat4 rotation_matrix(const float &x, const float &y, const float &z);
mat4 lookAt(const vec3& camera, const vec3& target, const vec3& up);

}