#include "body.h"

#include "vec3.h"

using cgmath::vec3;

//bool body::is_inside(const vec3& point) const {
//	vec3 min;
//	vec3 max;
//
//	for (int i = 0; i < 3; i++) {
//		min[i] = position[i] - dimensions[i] / 2.0f;
//		max[i] = position[i] + dimensions[i] / 2.0f;
//	}
//
//	if (min.x <= point.x && point.x <= max.x &&
//		min.y <= point.y && point.y <= max.y &&
//		min.z <= point.z && point.z <= max.z) {
//		return true;
//	}
//	return false;
//}
//
//bool body::intersect(const body& other) const {
//
//	vec3 points[8] = {
//		{position.x					, position.y				, position.z				},
//		{position.x					, position.y				, position.z + dimensions.z	},
//		{position.x					, position.y + dimensions.y	, position.z				},
//		{position.x					, position.y + dimensions.y	, position.z + dimensions.z	},
//		{position.x + dimensions.x	, position.y				, position.z				},
//		{position.x + dimensions.x	, position.y				, position.z + dimensions.z	},
//		{position.x + dimensions.x	, position.y + dimensions.y	, position.z				},
//		{position.x + dimensions.x	, position.y + dimensions.y	, position.z + dimensions.z	},
//	};
//
//	for (int i = 0; i < 8; i++) {
//		if (other.is_inside(points[i])) {
//			return true;
//		}
//	}
//	return false;
//}

bool body::check_collision(const body &b1, const body& b2) {
	vec3 minA, minB;
	vec3 maxA, maxB;

	for (int i = 0; i < 3; i++) {
		minA[i] = b1.position[i];
		maxA[i] = b1.position[i] + b1.dimensions[i];

		minB[i] = b2.position[i];
		maxB[i] = b2.position[i] + b2.dimensions[i];

		if (minA[i] > maxB[i] || maxA[i] < minB[i]) {
			return false;
		}
	}

	return true;
}


//void body::kinematic_static_collision(body &kinematic, const body &static_body, const vec3 &point) {
//	vec3 direction = -kinematic.velocity;
//
//	vec3 t_max, t_delta;
//	vec3 end = point + direction;
//	end = vec3(int(end.x), int(end.y), int(end.z));
//
//	vec3 step = vec3(sign(ray.x), sign(ray.y), sign(ray.z));
//
//	float border;
//	for (int i = 0; i < 3; i++) {
//		if (step[i] > 0.0) {
//			border = floor(origin[i] + 1.0);
//		}
//		else {
//			border = ceil(origin[i] - 1.0);
//		}
//		if (AreEqual(ray[i], 0.0)) {
//			t_max[i] = 100000.0;
//			t_delta[i] = 100000.0;
//		}
//		else {
//			t_max[i] = (border - origin[i]) / ray[i];
//			t_delta[i] = step[i] / ray[i];
//		}
//	}
//}
//
//void body::handle_collision(body &b1, body &b2) {
//	vec3 points[8] = {
//		{b1.position.x - b1.dimensions.x, b1.position.y - b1.dimensions.y, b1.position.z - b1.dimensions.z},
//		{b1.position.x - b1.dimensions.x, b1.position.y - b1.dimensions.y, b1.position.z + b1.dimensions.z},
//		{b1.position.x - b1.dimensions.x, b1.position.y + b1.dimensions.y, b1.position.z - b1.dimensions.z},
//		{b1.position.x - b1.dimensions.x, b1.position.y + b1.dimensions.y, b1.position.z + b1.dimensions.z},
//		{b1.position.x + b1.dimensions.x, b1.position.y - b1.dimensions.y, b1.position.z - b1.dimensions.z},
//		{b1.position.x + b1.dimensions.x, b1.position.y - b1.dimensions.y, b1.position.z + b1.dimensions.z},
//		{b1.position.x + b1.dimensions.x, b1.position.y + b1.dimensions.y, b1.position.z - b1.dimensions.z},
//		{b1.position.x + b1.dimensions.x, b1.position.y + b1.dimensions.y, b1.position.z + b1.dimensions.z},
//	};
//
//	for (int i = 0; i < 8; i++) {
//		if (b2.is_inside(points[i])) {
//			if (b1.is_kinematic) {
//				if (!b2.is_kinematic) {
//					kinematic_static_collision(b1, b2, points[i]);
//				}
//			}
//			else {
//				if (b2.is_kinematic) {
//					kinematic_static_collision(b2, b1, points[i]);
//				}
//			}
//		}
//	}
//}
//
//void body::update() {
//
//}
