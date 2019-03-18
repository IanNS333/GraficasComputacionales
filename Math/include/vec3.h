#pragma once

#include <iostream>

using std::ostream;

namespace cgmath {

class vec3 {
public:

	vec3();
	vec3(float x, float y, float z);

	float& operator[](int i);
	const float& operator[](int i) const;

	vec3& operator*=(float s);
	vec3& operator/=(float s);

	vec3& operator+=(const vec3& v);
	vec3& operator-=(const vec3& v);

	bool operator==(const vec3& v) const;

	float magnitude() const;
	void normalize();

	static float magnitude(const vec3& v);
	static vec3 normalize(const vec3& v);
	static float dot(const vec3& v1, const vec3& v2);
	static vec3 cross(const vec3& v1, const vec3& v2);

	friend ostream& operator<<(ostream& os, const vec3& v);

	float x;
	float y;
	float z;
};


inline vec3 operator*(const vec3& v, float s) {
	return vec3(v.x*s, v.y*s, v.z*s);
}

inline vec3 operator*(float s, const vec3& v) {
	return vec3(v.x*s, v.y*s, v.z*s);
}

inline vec3 operator/(const vec3& v, float s) {
	return vec3(v.x / s, v.y / s, v.z / s);
}

inline vec3 operator+(const vec3& v1, const vec3& v2) {
	return vec3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

inline vec3 operator-(const vec3& v1, const vec3& v2) {
	return vec3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

inline vec3 operator-(const vec3& v) {
	return vec3(-v.x, -v.y, -v.z);
}

}