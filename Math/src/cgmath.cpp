#include "cgmath.h"

namespace cgmath {

bool AreEqual(float a, float b)
{
	return fabsf(a - b) < std::numeric_limits<float>::epsilon();
}

}