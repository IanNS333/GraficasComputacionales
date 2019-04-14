#include <iostream>
#include <vector>

#include "mat4.h"
#include "mat3.h"
#include "cgmath.h"

using std::ostream;
using std::vector;

namespace cgmath {

mat4::mat4() : n{ 0 } {}

mat4::mat4(float diagonal) : n{
	{diagonal, 0, 0, 0},
	{0, diagonal, 0, 0},
	{0, 0, diagonal, 0},
	{0, 0, 0, diagonal} } {}

mat4::mat4(const vec4& c1, const vec4& c2, const vec4& c3, const vec4& c4) : n{
	{c1.x, c1.y, c1.z, c1.w},
	{c2.x, c2.y, c2.z, c2.w},
	{c3.x, c3.y, c3.z, c3.w},
	{c4.x, c4.y, c4.z, c4.w} } {}

vec4& mat4::operator[](int column) {
	return *reinterpret_cast<vec4*>(n[column]);
}

const vec4& mat4::operator[](int column) const {
	return *reinterpret_cast<const vec4*>(n[column]);
}

bool mat4::operator==(const mat4& m) const {
	return this->operator[](0) == m[0] &&
		this->operator[](1) == m[1] &&
		this->operator[](2) == m[2];
}

mat3 mat4::to_mat3() const {
	return mat3(
		vec3(this->operator[](0)),
		vec3(this->operator[](1)),
		vec3(this->operator[](2))
	);
}

float mat4::determinant(const mat4& m) {
	float result = 0;

	vector<int> is;
	vector<int> js = {1,2,3};
	mat3 current;

	for (int k = 0; k < 4; k++) {

		is = { 0,1,2,3 };
		is.erase(is.begin() + k);
		
		current = mat3();
		
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				current[i][j] = m[is[i]][js[j]];
			}
		}

		result += ((k %2 == 0) ? 1.0f : -1.0f) * m[k][0]*mat3::determinant(current);
	}

	return result;
}

// Gauss-elimination method
mat4 mat4::inverse(const mat4& original) {
	mat4 result(1);
	mat4 m;
	vec4 swap;

	float actual;

	for (int i = 0; i < 4; i++) {
		m[i] = original[i];
	}

	for (int j = 0; j < 4; j++) {
		if (m[j][j] == 0) {
			for (int i = 0; i < 4; i++) {
				if (m[i][j] != 0) {

					swap = m[j];
					m[j] = m[i];
					m[i] = swap;

					swap = result[j];
					result[j] = result[i];
					result[i] = swap;
				}
			}
		}
		for (int i = 0; i < 4; i++) {
			actual = m[i][j];
			if (i == j) {
				continue;
			}
			else {
				m[i] -= (actual * m[j]) / m[j][j];
				result[i] -= (actual * result[j]) / m[j][j];
			}
		}
	}
	for (int i = 0; i < 4; i++) {
		actual = m[i][i];
		m[i] /= actual;
		result[i] /= actual;
	}

	return result;
}

mat4 mat4::transpose(const mat4& m) {
	mat4 res;
	for (int j = 0; j < 4; j++) {
		for (int i = 0; i < 4; i++) {
			res[i][j] = m[j][i];
		}
	}
	return res;
}

ostream& operator<<(ostream& os, const mat4& m) {
	os << m[0][0] << " " << m[1][0] << " " << m[2][0] << " " << m[3][0]<< "\n"
		<< m[0][1] << " " << m[1][1] << " " << m[2][1] << " " << m[3][1] << "\n"
		<< m[0][2] << " " << m[1][2] << " " << m[2][2] << " " << m[3][2] << "\n"
		<< m[0][3] << " " << m[1][3] << " " << m[2][3] << " " << m[3][3];
	return os;
}

}