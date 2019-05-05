#define _USE_MATH_DEFINES
#include <cmath>

#include "cgmath.h"
#include "vec3.h"
#include "mat3.h"
#include "mat4.h"

namespace cgmath {

bool AreEqual(float a, float b)
{
	return fabsf(a - b) < std::numeric_limits<float>::epsilon();
}

float radians(float degrees) {
	return degrees * ((float)M_PI / 180.0f);
}

mat4 rotate_around(const float &radians, const vec3 &v) {
	vec3 u = vec3::normalize(v);

	float uxx = u.x * u.x;
	float uxy = u.x * u.y;
	float uxz = u.x * u.z;
	float uyy = u.y * u.y;
	float uyz = u.y * u.z;
	float uzz = u.z * u.z;

	float cosAngle = cosf(radians);
	float sinAngle = sinf(radians);
	
	return mat4::transpose({
		{cosAngle + uxx * (1.0f - cosAngle)			, uxy * (1.0f - cosAngle) - u.z * sinAngle	, uxz * (1.0f - cosAngle) + u.y * sinAngle	, 0.0f},
		{uxy * (1.0f - cosAngle) + u.z * sinAngle	, cosAngle + uyy*(1.0f-cosAngle)			, uyz * (1.0f - cosAngle) - u.x * sinAngle	, 0.0f},
		{uxz * (1.0f - cosAngle) - u.y * sinAngle	, uyz * (1.0f - cosAngle) + u.x * sinAngle	, cosAngle + uzz * (1.0f-cosAngle)			, 0.0f},
		{0.0f										, 0.0f										, 0.0f										, 1.0f}
	});
}

mat4 translation_matrix(const float &x, const float &y, const float &z) {
	return mat4::transpose({
		{ 1.0f, 0.0f, 0.0f, x },
		{ 0.0f, 1.0f, 0.0f, y },
		{ 0.0f, 0.0f, 1.0f, z },
		{ 0.0f, 0.0f, 0.0f, 1.0f }
	});
}

mat4 translation_matrix(const vec3 &v) {
	return translation_matrix(v.x, v.y, v.z);
}

mat4 rotation_matrix(const float &x, const float &y, const float &z) {
	mat4 rotationX = mat4::transpose({
	{1.0f, 0.0f, 0.0f, 0.0f},
	{0.0f, cos(x), -sin(x), 0.0f},
	{0.0f, sin(x), cos(x), 0.0f},
	{0.0f, 0.0f, 0.0f, 1.0f}
		});
	mat4 rotationY = mat4::transpose({
		{cos(y), 0.0f, sin(y), 0.0f},
		{0.0f, 1.0f, 0.0f, 0.0f},
		{-sin(y), 0.0f, cos(y), 0.0f},
		{0.0f, 0.0f, 0.0f, 1.0f}
		});
	mat4 rotationZ = mat4::transpose({
		{cos(z), -sin(z), 0.0f, 0.0f},
		{sin(z), cos(z), 0.0f, 0.0f},
		{0.0f, 0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 0.0f, 1.0f}
		});
	
	return rotationZ * rotationY * rotationX;
}

mat4 rotation_matrix(const vec3 &v) {
	return rotation_matrix(v.x, v.y, v.z);
}

//mat4 lookAt(const vec3 &camera, const vec3 &target, const vec3 &up) {
//	vec3 f = vec3::normalize(target - camera);
//	vec3 u = vec3::normalize(up);
//	vec3 s = vec3::normalize(vec3::cross(f, u));
//	u = vec3::cross(s, f);
//	
//	return mat4::transpose(mat4(
//		{ s.x, s.y, s.z, -vec3::dot(s, camera)},
//		{ u.x, u.y, u.z, -vec3::dot(u, camera) },
//		{ -f.x, -f.y, -f.z, vec3::dot(f, camera) },
//		{0.0f, 0.0f, 0.0f, 1.0f}
//	));
//}

mat4 lookAt(const vec3 &camera, const vec3 &target, const vec3 &up) {
	vec3 z = vec3::normalize(camera - target);
	vec3 x = vec3::normalize(vec3::cross(up, z));
	vec3 y = vec3::normalize(vec3::cross(z, x));

	mat4 orientation = {
		{x.x, y.x, z.x, 0.0f},
		{x.y, y.y, z.y, 0.0f},
		{x.z, y.z, z.z, 0.0f},
		{0.0f, 0.0f, 0.0f, 1.0f},
	};
	mat4 translation = {
		{ 1.0f, 0.0f, 0.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f, 0.0f },
		{ -camera.x, -camera.y, -camera.z, 1.0f }
	};

	return orientation * translation;
}

}