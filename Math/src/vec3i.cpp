#include <iostream>
#include <math.h>

#include "cgmath.h"
#include "vec3i.h"
#include "vec3.h"

using std::ostream;

namespace cgmath {

	vec3i::vec3i() : x(0), y(0), z(0) {}

	vec3i::vec3i(int x, int y, int z) : x(x), y(y), z(z) {}

	vec3i::vec3i(const vec3 &v) : x(v.x), y(v.y), z(v.z) {}

	int& vec3i::operator[](int i) {
		return (&x)[i];
	}

	const int& vec3i::operator[](int i) const {
		return (&x)[i];
	}

	vec3i& vec3i::operator*=(int s) {
		x *= s;
		y *= s;
		z *= s;
		return *this;
	}

	vec3i& vec3i::operator/=(int s) {
		x /= s;
		y /= s;
		z /= s;
		return *this;
	}

	vec3i& vec3i::operator+=(const vec3i& v) {
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	vec3i& vec3i::operator-=(const vec3i& v) {
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	bool vec3i::operator==(const vec3i& v) const {
		return AreEqual(x, v.x) &&
			AreEqual(y, v.y) &&
			AreEqual(z, v.z);
	}

	int vec3i::magnitude() const {
		return sqrt(x * x + y * y + z * z);
	}

	void vec3i::normalize() {
		this->operator/=(this->magnitude());
	}

	int vec3i::magnitude(const vec3i& v) {
		return v.magnitude();
	}

	vec3i vec3i::normalize(const vec3i& v) {
		return v / v.magnitude();
	}

	int vec3i::dot(const vec3i& v1, const vec3i& v2) {
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}

	vec3i vec3i::cross(const vec3i& v1, const vec3i& v2) {
		return vec3i(
			v1.y * v2.z - v2.y * v1.z,
			-(v1.x * v2.z - v2.x * v1.z),
			v1.x * v2.y - v2.x * v1.y);
	}

	ostream& operator<<(ostream& os, const vec3i& v) {
		os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
		return os;
	}

}