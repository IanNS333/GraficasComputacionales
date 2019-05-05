#pragma once

#include "hitable.h"
#include "u_vec3.h"

using utils::u_vec3;

class sphere : public hitable {
	public:
		sphere() {}
		sphere(u_vec3 cen, float r, material *ptr) : center(cen), radius(r), mat_ptr(ptr) {};
		virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;
		
		u_vec3 center;
		float radius;
		material* mat_ptr;
};