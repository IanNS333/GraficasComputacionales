#pragma once

#include "utils.h"
#include "time_utils.h"

namespace utils {

bool __randomInitialized__ = false;

float randf() {
	if (!__randomInitialized__) {
		std::srand(hr_clock::now().time_since_epoch().count() % INT_MAX);
		__randomInitialized__ = true;
	}
	return float(std::rand()) / float(RAND_MAX + 1.0);
}

u_vec3 random_point_in_unit_disk() {
	u_vec3 p;
	do {
		p = 2.0*u_vec3(randf(), randf(), 0) - u_vec3(1, 1, 0);
	} while (dot(p, p) >= 1.0);
	return p;
}

u_vec3 random_point_in_unit_sphere() {
	u_vec3 p;
	do {
		p = 2.0*u_vec3(randf(), randf(), randf()) - u_vec3(1, 1, 1);
	} while (p.squared_length() >= 1.0);

	return p;
}

u_vec3 reflect(const u_vec3& v, const u_vec3& normal) {
	return v - 2 * dot(v, normal)*normal;
}

bool refract(const u_vec3& v, const u_vec3& normal, float ni_over_nt, u_vec3& refracted) {
	u_vec3 uv = unit_vector(v);
	float dt = dot(uv, normal);
	float discriminant = 1.0 - ni_over_nt * ni_over_nt*(1 - dt * dt);
	if (discriminant > 0) {
		refracted = ni_over_nt * (uv - normal * dt) - normal * sqrt(discriminant);
		return true;
	}
	return false;
}

float schlick(float cosine, float ref_idx) {
	float r0 = (1 - ref_idx) / (1 + ref_idx);
	r0 = r0 * r0;
	return r0 + (1 - r0) * pow((1 - cosine), 5);
}

}// utils namespace