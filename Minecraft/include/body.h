#pragma once

#include "vec3.h"

using cgmath::vec3;

class body {
private:

	//void static kinematic_static_collision(body &kinematic, const body &static_body, const vec3 &point);
	//void static kinematic_kinematic_collision(body &k1, const body &k2, const vec3 &point);

	//bool is_inside(const vec3& point) const;
	//bool intersect(const body& other) const;

public:

	//body(vec3 pos, vec3 dim, vec3 vel, bool is_k = false, bool has_g = false) :
	//	position(pos), dimensions(dim), velocity(vel), is_kinematic(is_k), has_gravity(has_g) {
	//}

	body(vec3 pos, vec3 dim, vec3 vel) :
		position(pos), dimensions(dim), velocity(vel) {
	}

	//void update();

	static bool check_collision(const body &b1, const body& b2);
	//static void handle_collision(body &b1, body &b2);

	vec3 position;
	vec3 dimensions;
	vec3 velocity;
};