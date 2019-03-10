#include <iostream>
#include <vector>

#include "mat3.h"
#include "cgmath.h"

using std::ostream;
using std::vector;

namespace cgmath {

mat3::mat3() : n{0} {}

mat3::mat3(float diagonal) : n{
	{diagonal, 0, 0},
	{0, diagonal, 0},
	{0, 0, diagonal}} {}

mat3::mat3(const vec3& c1, const vec3& c2, const vec3& c3) : n{ 
	{c1.x, c1.y, c1.z},
	{c2.x, c2.y, c2.z},
	{c3.x, c3.y, c3.z}} {}

vec3& mat3::operator[](int column) {
	return *reinterpret_cast<vec3*>(n[column]);
}

const vec3& mat3::operator[](int column) const {
	return *reinterpret_cast<const vec3*>(n[column]);
}

bool mat3::operator==(const mat3& m) const {
	return this->operator[](0) == m[0] && 
		this->operator[](1) == m[1] && 
		this->operator[](2) == m[2];
}

float mat3::determinant(const mat3& m) {
	return (
		m[0][0] * (m[1][1] * m[2][2] - m[2][1] * m[1][2])
	 - (m[1][0] * (m[0][1] * m[2][2] - m[2][1] * m[0][2]))
	  + m[2][0] * (m[0][1] * m[1][2] - m[1][1] * m[0][2])
	);
}

// Gauss-elimination method
mat3 mat3::inverse(const mat3& original) {
	mat3 result(1);
	mat3 m;
	vec3 swap;

	float actual;

	for (int i = 0; i < 3; i++) {
		m[i] = original[i];
	}

	for (int j = 0; j < 3; j++) {
		if (m[j][j] == 0) {
			for (int i = 0; i < 3; i++) {
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
		for (int i = 0; i < 3; i++) {
			actual = m[i][j];
			if (i == j) {
				continue;
			}
			else {
				m[i] -= actual * (m[j] / m[j][j]);
				result[i] -= actual* (result[j] / m[j][j]);
			}
		}
	}
	for (int i = 0; i < 3; i++) {
		actual = m[i][i];
		m[i] /= actual;
		result[i] /= actual;
	}

	return result;
}

mat3 mat3::transpose(const mat3& m) {
	mat3 res;
	for (int j = 0; j < 3; j++) {
		for (int i = 0; i < 3; i++) {
			res[i][j] = m[j][i];
		}
	}
	return res;
}

ostream& operator<<(ostream& os, const mat3& m) {
	os  << m[0][0] << " " << m[1][0] << " " << m[2][0] << "\n"
		<< m[0][1] << " " << m[1][1] << " " << m[2][1] << "\n"
		<< m[0][2] << " " << m[1][2] << " " << m[2][2];
	return os;
}

}