#pragma once

#include "vec3.h"

using utils::vec3;

namespace utils 
{

float randf(); 
vec3 random_point_in_unit_disk();
vec3 random_point_in_unit_sphere();
vec3 reflect(const vec3& v, const vec3& normal);
bool refract(const vec3& v, const vec3& normal, float ni_over_nt, vec3& refracted);
float schlick(float cosine, float ref_idx);

}