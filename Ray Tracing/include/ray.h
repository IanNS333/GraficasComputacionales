#pragma once

#include "u_vec3.h"

using utils::u_vec3;

class ray
{
	public:
		ray() {}
		ray(const u_vec3& a, const u_vec3& b) {
			A = a;
			B = b;
		}
		u_vec3 origin() const { return A; }
		u_vec3 direction() const { return B; }
		u_vec3 point_at(float t) const {
			return A + t * B;
		}

		u_vec3 A;
		u_vec3 B;
};