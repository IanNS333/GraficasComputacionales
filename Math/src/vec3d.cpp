#include <iostream>
#include <math.h>

#include "cgmath.h"
#include "vec3d.h"
#include "vec3.h"

using std::ostream;

namespace cgmath {

	vec3d::vec3d() : x(0.0), y(0.0), z(0.0) {}

	vec3d::vec3d(double x, double y, double z) : x(x), y(y), z(z) {}

	vec3d::vec3d(const vec3 &v) : x(v.x), y(v.y), z(v.z) {}

	double& vec3d::operator[](int i) {
		return (&x)[i];
	}

	const double& vec3d::operator[](int i) const {
		return (&x)[i];
	}

	vec3d& vec3d::operator*=(double s) {
		x *= s;
		y *= s;
		z *= s;
		return *this;
	}

	vec3d& vec3d::operator/=(double s) {
		x /= s;
		y /= s;
		z /= s;
		return *this;
	}

	vec3d& vec3d::operator+=(const vec3d& v) {
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	vec3d& vec3d::operator-=(const vec3d& v) {
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	bool vec3d::operator==(const vec3d& v) const {
		return AreEqual(x, v.x) &&
			AreEqual(y, v.y) &&
			AreEqual(z, v.z);
	}

	double vec3d::magnitude() const {
		return sqrt(x * x + y * y + z * z);
	}

	void vec3d::normalize() {
		this->operator/=(this->magnitude());
	}

	double vec3d::magnitude(const vec3d& v) {
		return v.magnitude();
	}

	vec3d vec3d::normalize(const vec3d& v) {
		return v / v.magnitude();
	}

	double vec3d::dot(const vec3d& v1, const vec3d& v2) {
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}

	vec3d vec3d::cross(const vec3d& v1, const vec3d& v2) {
		return vec3d(
			v1.y * v2.z - v2.y * v1.z,
			-(v1.x * v2.z - v2.x * v1.z),
			v1.x * v2.y - v2.x * v1.y);
	}

	ostream& operator<<(ostream& os, const vec3d& v) {
		os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
		return os;
	}

}