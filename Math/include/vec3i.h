#pragma once

#include <iostream>

#include "vec3.h"

using std::ostream;

namespace cgmath {

	class vec3i {
		public:

		vec3i();
		vec3i(int x, int y, int z);
		vec3i(const vec3 &v);

		int& operator[](int i);
		const int& operator[](int i) const;

		vec3i& operator*=(int s);
		vec3i& operator/=(int s);

		vec3i& operator+=(const vec3i& v);
		vec3i& operator-=(const vec3i& v);

		bool operator==(const vec3i& v) const;

		int magnitude() const;
		void normalize();

		static int magnitude(const vec3i& v);
		static vec3i normalize(const vec3i& v);
		static int dot(const vec3i& v1, const vec3i& v2);
		static vec3i cross(const vec3i& v1, const vec3i& v2);

		friend ostream& operator<<(ostream& os, const vec3i& v);

		int x;
		int y;
		int z;
	};

	inline vec3i operator*(const vec3i& v, int s) {
		return vec3i(v.x*s, v.y*s, v.z*s);
	}

	inline vec3i operator*(int s, const vec3i& v) {
		return vec3i(v.x*s, v.y*s, v.z*s);
	}

	inline vec3i operator/(const vec3i& v, int s) {
		return vec3i(v.x / s, v.y / s, v.z / s);
	}

	inline vec3i operator+(const vec3i& v1, const vec3i& v2) {
		return vec3i(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
	}

	inline vec3i operator-(const vec3i& v1, const vec3i& v2) {
		return vec3i(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
	}

	inline vec3i operator-(const vec3i& v) {
		return vec3i(-v.x, -v.y, -v.z);
	}
}