#pragma once

#include "ray.h"
#include "u_vec3.h"

using utils::u_vec3;

class material;

struct  hit_record {
	float t;
	u_vec3 p;
	u_vec3 normal;
	material *mat_ptr;
};

class hitable {
	public:
		virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const = 0;
};