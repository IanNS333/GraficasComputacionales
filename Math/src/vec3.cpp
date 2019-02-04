#include <iostream>
#include <math.h>

#include "cgmath.h"
#include "vec3.h"

using std::ostream;

namespace cgmath {

vec3::vec3() : x(0.0f), y(0.0f), z(0.0f) {}

vec3::vec3(float x, float y, float z) : x(x), y(y), z(z) {}

float& vec3::operator[](int i) {
	return (&x)[i];
}

const float& vec3::operator[](int i) const {
	return (&x)[i];
}

vec3& vec3::operator*=(float s) {
	x *= s;
	y *= s;
	z *= s;
	return *this;
}

vec3& vec3::operator/=(float s) {
	x /= s;
	y /= s;
	z /= s;
	return *this;
}

vec3& vec3::operator+=(const vec3& v) {
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

vec3& vec3::operator-=(const vec3& v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

bool vec3::operator==(const vec3& v) const {
	return AreEqual(x, v.x) &&
		AreEqual(y, v.y) &&
		AreEqual(z, v.z);
}

float vec3::magnitude() const {
	return sqrtf(x * x + y * y + z * z);
}

void vec3::normalize() {
	this->operator/=(this->magnitude());
}

float vec3::magnitude(const vec3& v) {
	return v.magnitude();
}

vec3 vec3::normalize(const vec3& v) {
	return v / v.magnitude();
}

float vec3::dot(const vec3& v1, const vec3& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}
	
vec3 vec3::cross(const vec3& v1, const vec3& v2) {
	return vec3(
		v1.y * v2.z - v2.y * v1.z,
		-(v1.x * v2.z - v2.x * v1.z),
		v1.x * v2.y - v2.x * v1.y);
}

ostream& operator<<(ostream& os, const vec3& v) {
	os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
	return os;
}

}