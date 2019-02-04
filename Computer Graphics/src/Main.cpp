#include <iostream>

#include "complex.h"
#include "mat3.h"
#include "vec3.h"

using cgmath::complex;
using cgmath::mat3;
using cgmath::vec3;
using std::cin;
using std::cout;
using std::endl;

int main(int argc, char* argv[])
{

	vec3 c1(1,0,0), 
		c2(0,0,1), 
		c3(0,1,0);

	mat3 m(c1,c2,c3);
	cout << m << "\n";
	mat3 inverse = mat3::inverse(m);
	cout << inverse << "\n";
	
	cout << "epsilon: " << std::numeric_limits<float>::epsilon() << endl;
	cout << "Press Any Key to continue." << endl;
	cin.get();


	return 0;
}