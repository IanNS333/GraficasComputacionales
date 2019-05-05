#pragma once

#include <math.h>
#include <iostream>

namespace utils {

class u_vec3 {
	public:
	u_vec3() {}
	u_vec3(float e0, float e1, float e2) {
		e[0] = e0;
		e[1] = e1;
		e[2] = e2;
	}

	// vector
	inline float x() const { return e[0]; }
	inline float y() const { return e[1]; }
	inline float z() const { return e[2]; }

	// color
	inline float r() const { return e[0]; }
	inline float g() const { return e[1]; }
	inline float b() const { return e[2]; }

	inline const u_vec3& operator+() const { return *this; }
	inline u_vec3 operator-() const { return u_vec3(-e[0], -e[1], -e[2]); }
	inline float operator[](int i) const { return e[i]; }
	inline float& operator[](int i) { return e[i]; }

	inline u_vec3& operator+=(const u_vec3 &v2);
	inline u_vec3& operator-=(const u_vec3 &v2);
	inline u_vec3& operator*=(const u_vec3 &v2);
	inline u_vec3& operator/=(const u_vec3 &v2);
	inline u_vec3& operator*=(const float t);
	inline u_vec3& operator/=(const float t);

	inline float length() const {
		return sqrt((e[0] * e[0]) + (e[1] * e[1]) + (e[2] * e[2]));
	}

	inline float squared_length() const {
		return (e[0] * e[0]) + (e[1] * e[1]) + (e[2] * e[2]);
	}

	inline void make_unit_vector();

	float e[3];
};

inline std::istream& operator>>(std::istream &is, u_vec3 &t) {
	is >> t.e[0] >> t.e[1] >> t.e[2];
	return is;
}

inline std::ostream& operator<<(std::ostream &os, const u_vec3 &t) {
	os << t.e[0] << " " << t.e[1] << " " << t.e[2];
	return os;
}

inline void u_vec3::make_unit_vector() {
	float k = 1.0f / sqrtf((e[0] * e[0]) + (e[1] * e[1]) + (e[2] * e[2]));
	e[0] *= k;
	e[1] *= k;
	e[2] *= k;
}

inline u_vec3 operator+(const u_vec3& v1, const u_vec3& v2) {
	return u_vec3(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2]);
}

inline u_vec3 operator-(const u_vec3& v1, const u_vec3& v2) {
	return u_vec3(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]);
}

inline u_vec3 operator*(const u_vec3& v1, const u_vec3& v2) {
	return u_vec3(v1.e[0] * v2.e[0], v1.e[1] * v2.e[1], v1.e[2] * v2.e[2]);
}

inline u_vec3 operator/(const u_vec3& v1, const u_vec3& v2) {
	return u_vec3(v1.e[0] / v2.e[0], v1.e[1] / v2.e[1], v1.e[2] / v2.e[2]);
}

inline u_vec3 operator*(float t, const u_vec3& v) {
	return u_vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline u_vec3 operator*(const u_vec3& v, float t) {
	return u_vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline u_vec3 operator/(const u_vec3& v, float t) {
	return u_vec3(v.e[0] / t, v.e[1] / t, v.e[2] / t);
}

inline float dot(const u_vec3& v1, const u_vec3& v2) {
	return (v1.e[0] * v2.e[0]) + (v1.e[1] * v2.e[1]) + (v1.e[2] * v2.e[2]);
}

inline u_vec3 cross(const u_vec3& v1, const u_vec3& v2) {
	return u_vec3(
		v1.e[1] * v2.e[2] - v1.e[2] * v2.e[1],
		-(v1.e[0] * v2.e[2] - v1.e[2] * v2.e[0]),
		v1.e[0] * v2.e[1] - v1.e[1] * v2.e[0]
	);
}

inline u_vec3 unit_vector(u_vec3 v) {
	return v / v.length();
}

inline u_vec3& u_vec3::operator+=(const u_vec3 &v) {
	e[0] += v.e[0];
	e[1] += v.e[1];
	e[2] += v.e[2];
	return *this;
}

inline u_vec3& u_vec3::operator-=(const u_vec3 &v) {
	e[0] -= v.e[0];
	e[1] -= v.e[1];
	e[2] -= v.e[2];
	return *this;
}

inline u_vec3& u_vec3::operator*=(const u_vec3 &v) {
	e[0] *= v.e[0];
	e[1] *= v.e[1];
	e[2] *= v.e[2];
	return *this;
}

inline u_vec3& u_vec3::operator/=(const u_vec3 &v) {
	e[0] /= v.e[0];
	e[1] /= v.e[1];
	e[2] /= v.e[2];
	return *this;
}

inline u_vec3& u_vec3::operator*=(const float t) {
	e[0] *= t;
	e[1] *= t;
	e[2] *= t;
	return *this;
}

inline u_vec3& u_vec3::operator/=(const float t) {
	float k = 1.0f / t;

	e[0] *= k;
	e[1] *= k;
	e[2] *= k;
	return *this;
}

}