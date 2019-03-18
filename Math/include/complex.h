#pragma once

#include<iostream>

namespace cgmath {

class complex {

public:
	float real;
	float imaginary;

	complex();
	complex(float r, float i);

	void print() const;
	friend std::ostream& operator<<(std::ostream&, const complex&);

	void add(const complex& other);
	complex& operator+=(const complex& other);

	static complex add(const complex& c1, const complex& c2);
};

inline complex operator+(const complex& c1, const complex& c2) {
	return complex(c1.real + c2.real, c1.imaginary + c2.imaginary);
}

}