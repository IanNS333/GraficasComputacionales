#pragma once

#include <iostream>

#include "vec3.h"

using std::ostream;

namespace cgmath {

	class vec3d {
		public:

		vec3d();
		vec3d(double x, double y, double z);
		vec3d(const vec3 &v);

		double& operator[](int i);
		const double& operator[](int i) const;

		vec3d& operator*=(double s);
		vec3d& operator/=(double s);

		vec3d& operator+=(const vec3d& v);
		vec3d& operator-=(const vec3d& v);

		bool operator==(const vec3d& v) const;

		double magnitude() const;
		void normalize();

		static double magnitude(const vec3d& v);
		static vec3d normalize(const vec3d& v);
		static double dot(const vec3d& v1, const vec3d& v2);
		static vec3d cross(const vec3d& v1, const vec3d& v2);

		friend ostream& operator<<(ostream& os, const vec3d& v);

		double x;
		double y;
		double z;
	};

	inline vec3d operator*(const vec3d& v, double s) {
		return vec3d(v.x*s, v.y*s, v.z*s);
	}

	inline vec3d operator*(double s, const vec3d& v) {
		return vec3d(v.x*s, v.y*s, v.z*s);
	}

	inline vec3d operator/(const vec3d& v, double s) {
		return vec3d(v.x / s, v.y / s, v.z / s);
	}

	inline vec3d operator+(const vec3d& v1, const vec3d& v2) {
		return vec3d(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
	}

	inline vec3d operator-(const vec3d& v1, const vec3d& v2) {
		return vec3d(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
	}

	inline vec3d operator-(const vec3d& v) {
		return vec3d(-v.x, -v.y, -v.z);
	}
}