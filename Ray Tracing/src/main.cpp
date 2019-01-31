#define _USE_MATH_DEFINES


#include <vector>
#include <limits.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <thread>

#include "image_writer.h"
#include "vec3.h"
#include "ray.h"
#include "hitable.h"
#include "hitable_list.h"
#include "sphere.h"
#include "camera.h"
#include "utils.h"
#include "lambertian.h"
#include "metal.h"
#include "dielectric.h"
#include "time_utils.h"

using utils::vec3;
using utils::unit_vector;
using utils::cross;
using utils::dot;
using utils::random_point_in_unit_sphere;
using utils::randf;

vec3 color(const ray& r, const hitable * const world, int depth) {
	hit_record rec;

	if (world->hit(r, 0.001, FLT_MAX, rec)) {
		ray scattered;
		vec3 attenuation;
		if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
			attenuation*color(scattered, world, depth + 1);
		}
		else {
			return vec3(0, 0, 0);
		}
	}
	else {
		// background color
		vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5*(unit_direction.y() + 1.0);
		return (1.0 - t)*vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
	}
}

hitable *random_scene() {
	int n = 25*25 + 4;

	hitable **list = new hitable*[n + 1];
	list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(vec3(0.5, 0.5, 0.5)));

	int i = 1;

	//25 * 25 spheres
	for (int a = -13; a < 12; a++) {
		for (int b = -13; b < 12; b++) {
			float choose_mat = randf();

			vec3 center(a + 0.9*randf(), 0.2, b + 0.9*randf());

			if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
				if (choose_mat < 0.35) {
					list[i++] = new sphere(center, 0.2, 
						new lambertian(
							vec3(
								randf()*randf(), 
								randf()*randf(), 
								randf()*randf()
							)
						)
					);
				}
				else if (choose_mat < 0.7) {
					list[i++] = new sphere(center, 0.2,
						new metal(
							vec3(
								0.5*(1+randf()),
								0.5*(1 + randf()),
								0.5*(1 + randf())
							),
							0.5 * randf()
						)
					);
				}
				else {
					list[i++] = new sphere(center, 0.2, new dielectric(1.5));
				}
			}
		}
	}

	list[i++] = new sphere(vec3(-6, 1, 0), 1.0, new dielectric(1.5));
	list[i++] = new sphere(vec3(-2, 1, 0), 1.0, new dielectric(1.5));
	list[i++] = new sphere(vec3(2, 1, 0), 1.0, new dielectric(1.5));
	list[i++] = new sphere(vec3(6, 1, 0), 1.0, new dielectric(1.5));

	return new hitable_list(list, i);
}

const int nx = 300;
const int ny = 200;
const int ns = 20;

void calculatePixel(int i, int j, const camera& cam, hitable* world ,unsigned char& r, unsigned char& g,unsigned char& b) {
	vec3 pix(0, 0, 0);

	for (int s = 0; s < ns; s++) {
		float u = float(i + randf()) / float(nx);
		float v = float(ny - (j + randf())) / float(ny);

		ray r = cam.get_ray(u, v);

		pix += color(r, world, 0);
	}

	pix /= float(ns);

	r = 255.99*sqrt(pix[0]);
	g = 255.99*sqrt(pix[1]);
	b = 255.99*sqrt(pix[2]);
}

int main() {

	time::init();

	std::vector<unsigned char> pixels(nx*ny*3); 

	hitable * world = random_scene();

	vec3 lookfrom(13, 2, 3);
	vec3 lookat(0, 0, 0);
	//float dist_to_focus = (lookfrom - lookat).length();
	float dist_to_focus = 10;
	float aperture = 0.1;

	camera cam(lookfrom, lookat, vec3(0,1,0), 20, float(nx)/float(ny), 0.1, dist_to_focus);
	
	std::thread threads[nx*ny];

	int count = 0;
	int countChannels = 0;
	for (int j = 0; j < ny; j++) {
		for (int i = 0; i < nx; i++) {
			int a = countChannels++;
			int b = countChannels++;
			int c = countChannels++;
			threads[count++] = std::thread([&](int a, int b, int c) {
				calculatePixel(i, j, cam, world, pixels[a], pixels[b], pixels[c]);
			}, a, b, c);
		}
	}

	for (auto& t : threads) {
		t.join();
	}

	image_writer::save_png("out.png", nx, ny, 3, pixels.data());

	std::ofstream myfile;
	myfile.open("out.txt");
	myfile << "Elapsed: " << time::elapsed_time().count() << " seconds.\n";
	myfile.close();

	return 0;
}