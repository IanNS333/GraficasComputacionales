#define _USE_MATH_DEFINES
#include <cmath>

#include "cgmath.h"

namespace cgmath {

bool AreEqual(float a, float b)
{
	return fabsf(a - b) < std::numeric_limits<float>::epsilon();
}

float radians(float degrees) {
	return degrees * ((float)M_PI / 180.0f);
}

}