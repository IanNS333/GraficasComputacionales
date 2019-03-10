#include <iostream>
#include <math.h>

#include "cgmath.h"
#include "vec2.h"

using std::ostream;

namespace cgmath {

vec2::vec2() : x(0.0f), y(0.0f) {}

vec2::vec2(float x, float y) : x(x), y(y) {}

float& vec2::operator[](int i) {
	return (&x)[i];
}

const float& vec2::operator[](int i) const{
	return (&x)[i];
}

vec2& vec2::operator*=(float s){
	x *= s;
	y *= s;
	return *this;
}

vec2& vec2::operator/=(float s){
	x /= s;
	y /= s;
	return *this;
}

vec2& vec2::operator+=(const vec2& v){
	x += v.x;
	y += v.y;
	return *this;
}

vec2& vec2::operator-=(const vec2& v){
	x -= v.x;
	y -= v.y;
	return *this;
}

bool vec2::operator==(const vec2& v) const{
	
	return AreEqual(x, v.x) && AreEqual(y, v.y);
}

float vec2::magnitude() const{
	return sqrtf(x * x + y * y);
}

void vec2::normalize(){
	this->operator/=(this->magnitude());
}

float vec2::magnitude(const vec2& v){
	return v.magnitude();
}

vec2 vec2::normalize(const vec2& v){
	return v / v.magnitude();
}

float vec2::dot(const vec2& v1, const vec2& v2){
	return v1.x * v2.x + v1.y * v2.y;
}

ostream& operator<<(ostream& os, const vec2& v){
	os << "(" << v.x << ", " << v.y << ")";
	return os;
}

}