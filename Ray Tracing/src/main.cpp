#include <vector>

#include "image_writer.h"
#include "vec3.h"

int main() {

	int nx = 400;
	int ny = 300;

	std::vector<unsigned char> pixels;

	for (int i = 0; i < ny; i++) {
		for (int j = 0; j < nx; j++) {

			vec3 pix = vec3(
				float(j) / float(nx),	// red
				float(i) / float(ny),	// green
				0.2						// blue
			);

			pixels.push_back(int(255.99 * pix[0]));
			pixels.push_back(int(255.99 * pix[1]));
			pixels.push_back(int(255.99 * pix[2]));
		}
	}

	image_writer::save_png("out.png", nx, ny, 3, pixels.data());

	return 0;
}