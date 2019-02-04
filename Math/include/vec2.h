#pragma once

#include <iostream>

using std::ostream;

namespace cgmath {

class vec2 {
	public:
		
		vec2();
		vec2(float x, float y);

		float& operator[](int i);
		const float& operator[](int i) const;

		vec2& operator*=(float s);
		vec2& operator/=(float s);

		vec2& operator+=(const vec2& v);
		vec2& operator-=(const vec2& v);

		bool operator==(const vec2& v) const;

		float magnitude() const;
		void normalize();

		static float magnitude(const vec2& v);
		static vec2 normalize(const vec2& v);
		static float dot(const vec2& v1, const vec2& v2);

		friend ostream& operator<<(ostream& os, const vec2& v);

		float x;
		float y;
};


inline vec2 operator*(const vec2& v, float s) {
	return vec2(v.x*s, v.y*s);
}

inline vec2 operator*(float s, const vec2& v) {
	return vec2(v.x*s, v.y*s);
}

inline vec2 operator/(const vec2& v, float s) {
	return vec2(v.x/s, v.y/s);
}

inline vec2 operator+(const vec2& v1, const vec2& v2) {
	return vec2(v1.x + v2.x, v1.y + v2.y);
}

inline vec2 operator-(const vec2& v1, const vec2& v2) {
	return vec2(v1.x - v2.x, v1.y - v2.y);
}

inline vec2 operator-(const vec2& v) {
	return vec2(- v.x, -v.y);
}

}