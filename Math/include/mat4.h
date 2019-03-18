#pragma once

#pragma once

#include <iostream>

#include "vec4.h"

using std::ostream;

namespace cgmath {

class mat4 {
private:
	float n[4][4] = { 0 };

public:
	mat4();
	mat4(float diagonal);
	mat4(const vec4& c1, const vec4& c2, const vec4& c3, const vec4& c4);

	vec4& operator[](int column);
	const vec4& operator[](int column) const;
	bool operator==(const mat4& m) const;

	static float determinant(const mat4& m);
	static mat4 inverse(const mat4& m);
	static mat4 transpose(const mat4& m);

	friend ostream& operator<<(ostream& os, const mat4&);
};

inline vec4 operator*(const mat4& m, const vec4& v) {
	mat4 transpose = mat4::transpose(m);
	vec4 result;

	for (int i = 0; i < 4; i++) {
		result[i] = vec4::dot(transpose[i], v);
	}

	return result;
}

inline mat4 operator*(const mat4& m1, const mat4& m2) {
	mat4 transpose = mat4::transpose(m1);
	mat4 result;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result[i][j] = vec4::dot(transpose[j], m2[i]);
		}
	}

	return result;
}

}