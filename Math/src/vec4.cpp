#include <iostream>
#include <math.h>

#include "cgmath.h"
#include "vec4.h"

using std::ostream;

namespace cgmath {

vec4::vec4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}

vec4::vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

float& vec4::operator[](int i) {
	return (&x)[i];
}

const float& vec4::operator[](int i) const {
	return (&x)[i];
}

vec4& vec4::operator*=(float s) {
	x *= s;
	y *= s;
	z *= s;
	w *= s;
	return *this;
}

vec4& vec4::operator/=(float s) {
	x /= s;
	y /= s;
	z /= s;
	w /= s;
	return *this;
}

vec4& vec4::operator+=(const vec4& v) {
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;
	return *this;
}

vec4& vec4::operator-=(const vec4& v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;
	return *this;
}

bool vec4::operator==(const vec4& v) const {
	return AreEqual(x, v.x) &&
		AreEqual(y, v.y) &&
		AreEqual(z, v.z) &&
		AreEqual(w, v.w);
}

float vec4::dot(const vec4& v1, const vec4& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}

ostream& operator<<(ostream& os, const vec4& v) {
	os << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
	return os;
}

}