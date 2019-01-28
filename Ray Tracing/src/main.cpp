#include <vector>
#include "image_writer.h"
#include <random>

int main() {

	int nx = 400;
	int ny = 300;

	std::vector<unsigned char> pixels;

	for (int i = 0; i < ny; i++) {
		for (int j = 0; j < nx; j++) {
			float r = float(j)/ float(nx);
			float g = float(i)/ float(ny);
			float b = 0;

			pixels.push_back(int(255.99 * r));
			pixels.push_back(int(255.99 * g));
			pixels.push_back(int(255.99 * b));
		}
	}

	image_writer::save_png("out.png", nx, ny, 3, pixels.data());

	return 0;
}