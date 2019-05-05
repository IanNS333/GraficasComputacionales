#pragma once

#include "hitable.h"
#include "ray.h"
#include "u_vec3.h"

using utils::u_vec3;

class material {
	public:
		virtual bool scatter(const ray& r_in, const hit_record& rec, u_vec3& attenuation, ray& scattered) const = 0;
};
