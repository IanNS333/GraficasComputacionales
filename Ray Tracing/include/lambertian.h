#pragma once

#include "material.h"
#include "utils.h"
#include "u_vec3.h"

using utils::u_vec3;

class lambertian : public material {
	public:
		lambertian(const u_vec3 &a) : albedo(a) {}
		virtual bool scatter(const ray& r_in, const hit_record& rec, u_vec3& attenuation, ray& scattered) const {
			u_vec3 target = rec.p + rec.normal + utils::random_point_in_unit_sphere();
			scattered = ray(rec.p, target - rec.p);
			attenuation = albedo;
			return true;
		}

		u_vec3 albedo;
};