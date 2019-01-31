#pragma once

#include "material.h"
#include "utils.h"
#include "vec3.h"

using utils::vec3;
using utils::unit_vector;
using utils::reflect;
using utils::random_point_in_unit_sphere;

class metal : public material {
	public:
		metal(const vec3& a, float f) : albedo(a) {
			if (f < 1) {
				fuzz = f;
			}
			else {
				fuzz = 1;
			}
		}
		virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const {
			vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
			scattered = ray(rec.p, reflected + fuzz*random_point_in_unit_sphere());
			attenuation = albedo;
			return (dot(scattered.direction(), rec.normal) > 0);
		}

		vec3 albedo;
		float fuzz;
};