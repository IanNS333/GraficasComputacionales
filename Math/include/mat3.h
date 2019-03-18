#pragma once

#include <iostream>

#include "vec3.h"

using std::ostream;

namespace cgmath {

class mat3 {
private:
	float n[3][3] = { 0 };

public:
	mat3();
	mat3(float diagonal);
	mat3(const vec3& c1, const vec3& c2, const vec3& c3);

	vec3& operator[](int column);
	const vec3& operator[](int column) const;
	bool operator==(const mat3& m) const;

	static float determinant(const mat3& m);
	static mat3 inverse(const mat3& m);
	static mat3 transpose(const mat3& m);

	friend ostream& operator<<(ostream& os, const mat3&);
};

inline vec3 operator*(const mat3& m, const vec3& v) {
	mat3 transpose = mat3::transpose(m);
	vec3 result;
	
	for (int i = 0; i < 3; i++) {
		result[i] = vec3::dot(transpose[i], v);
	}

	return result;
}

inline mat3 operator*(const mat3& m1, const mat3& m2) {
	mat3 transpose = mat3::transpose(m1);
	mat3 result;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			result[i][j] = vec3::dot(transpose[j], m2[i]);
		}
	}

	return result;
}

}