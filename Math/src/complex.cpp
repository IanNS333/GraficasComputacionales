#include "complex.h"

namespace cgmath{

complex::complex() : real(0.0), imaginary(0.0)
{
}

complex::complex(float r, float i) : real(r), imaginary(i) 
{
} 

void complex::print() const {
	std::cout << real << " + " << imaginary << "i" << std::endl;
}

void complex::add(const complex& other) {
	real += other.real;
	imaginary += other.imaginary;
}

complex& complex::operator+=(const complex& other) {
	real += other.real;
	imaginary += other.imaginary;
	return *this;
}

complex complex::add(const complex& c1, const complex& c2) {
	return complex(c1.real + c2.real, c1.imaginary + c2.imaginary);
}

std::ostream & operator<<(std::ostream & os, const complex & c)
{
	os << c.real << " + " << c.imaginary << "i";
	return os;
}

}
