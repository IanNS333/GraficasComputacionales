#pragma once

#define _USE_MATH_DEFINES

#include <math.h>
#include "ray.h"
#include "utils.h"
#include "u_vec3.h"

using utils::random_point_in_unit_disk;

using utils::u_vec3;

class camera {
	public:
		camera(u_vec3 lookfrom, u_vec3 lookat, u_vec3 vup, float vfov, float aspect, float aperture, float focus_dist) { // vfov is top to bottom in degrees

			lens_radius = aperture / 2;

			float theta = vfov * M_PI / 180;
			float half_height = tan(theta / 2);
			float half_width = aspect * half_height;
			
			origin = lookfrom;
			
			w = unit_vector(lookfrom - lookat);
			u = unit_vector(cross(vup, w));
			v = cross(w, u);

			lowerLeftCorner = origin - half_width * focus_dist * u - half_height * focus_dist * v - focus_dist * w;
			horizontal = 2*half_width * focus_dist * u;
			vertical = 2*half_height * focus_dist * v;
		}

		ray get_ray(float s, float t) const {
			u_vec3 rd = lens_radius * random_point_in_unit_disk();
			u_vec3 offset = u * rd.x() + v * rd.y();
			return ray(origin + offset, lowerLeftCorner + s * horizontal + t * vertical - origin - offset);
		}

		u_vec3 lowerLeftCorner;
		u_vec3 horizontal;
		u_vec3 vertical;
		u_vec3 origin;
		u_vec3 u, v, w;
		float lens_radius;
};