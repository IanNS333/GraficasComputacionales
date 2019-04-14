#pragma once

#include "u_vec3.h"

namespace utils 
{

float randf(); 
u_vec3 random_point_in_unit_disk();
u_vec3 random_point_in_unit_sphere();
u_vec3 reflect(const u_vec3& v, const u_vec3& normal);
bool refract(const u_vec3& v, const u_vec3& normal, float ni_over_nt, u_vec3& refracted);
float schlick(float cosine, float ref_idx);

}