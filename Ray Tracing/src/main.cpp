#define _USE_MATH_DEFINES


#include <vector>
#include <limits.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <thread>
#include <stdio.h>
#include <mutex>

#include "image_writer.h"
#include "u_vec3.h"
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

using utils::u_vec3;
using utils::unit_vector;
using utils::cross;
using utils::dot;
using utils::random_point_in_unit_sphere;
using utils::randf;
using std::vector;
using std::thread;

u_vec3 color(const ray& r, const hitable * const world, int depth) {
	hit_record rec;

	if (world->hit(r, 0.001, FLT_MAX, rec)) {
		ray scattered;
		u_vec3 attenuation;
		if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
			return attenuation*color(scattered, world, depth + 1);
		}
		else {
			return u_vec3(0, 0, 0);
		}
	}
	else {
		// background color
		u_vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5*(unit_direction.y() + 1.0);
		return (1.0 - t)*u_vec3(1.0, 1.0, 1.0) + t * u_vec3(0.5, 0.7, 1.0);
	}
}

hitable *random_scene() {
	int n = 25*25 + 4;

	hitable **list = new hitable*[n + 1];
	list[0] = new sphere(u_vec3(0, -1000, 0), 1000, new lambertian(u_vec3(0.5, 0.5, 0.5)));

	int i = 1;

	//25 * 25 spheres
	for (int a = -13; a < 12; a++) {
		for (int b = -13; b < 12; b++) {
			float choose_mat = randf();

			u_vec3 center(a + 0.9*randf(), 0.2, b + 0.9*randf());

			if ((center - u_vec3(4, 0.2, 0)).length() > 0.9) {
				if (choose_mat < 0.35) {
					list[i++] = new sphere(center, 0.2, 
						new lambertian(
							u_vec3(
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
							u_vec3(
								0.5*(1 + randf()),
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

	list[i++] = new sphere(u_vec3(-10, 1, 1), 1.0, new dielectric(1.5));
	list[i++] = new sphere(u_vec3(-6, 1, 0.5), 1.0, new dielectric(1.5));
	list[i++] = new sphere(u_vec3(-2, 1, 0), 1.0, new dielectric(1.5));
	list[i++] = new sphere(u_vec3(2, 1, -0.5), 1.0, new dielectric(1.5));

	return new hitable_list(list, i);
}

int nx = 1920;
int ny = 1080;
int ns = 100;
int rowCountCompleted = 0;
std::mutex mtx;
vector<unsigned char> pixels(nx*ny * 3);


u_vec3 calculatePixel(int i, int j, const camera& cam, hitable* world) {
	u_vec3 pix(0, 0, 0);

	for (int s = 0; s < ns; s++) {
		float u = float(i + randf()) / float(nx);
		float v = float(ny - (j + randf())) / float(ny);

		ray r = cam.get_ray(u, v);

		pix += color(r, world, 0);
	}

	pix /= float(ns);

	pix[0] = 255.99*sqrt(pix[0]);
	pix[1] = 255.99*sqrt(pix[1]);
	pix[2] = 255.99*sqrt(pix[2]);

	return pix;
}

void calculatePixelRow(int j, const camera& cam, hitable * world, int result_start) {
	for (int i = 0; i < nx; i++) {
		u_vec3 result = calculatePixel(i, j, cam, world);
		pixels[result_start++] = result.r();
		pixels[result_start++] = result.g();
		pixels[result_start++] = result.b();
	}
	mtx.lock();
	printf("%0.2f %% finished!\n", float(++rowCountCompleted)/float(ny) * 100);
	mtx.unlock();
}

int main() {

	time::init();

	hitable * world = random_scene();

	u_vec3 lookfrom(13, 2, 3);
	u_vec3 lookat(0, 0, 0);
	//float dist_to_focus = (lookfrom - lookat).length();
	float dist_to_focus = 10;
	float aperture = 0.1;

	camera cam(lookfrom, lookat, u_vec3(0,1,0), 20, float(nx)/float(ny), aperture, dist_to_focus);

	vector<thread> threads(ny);

	int threadCount = 0;

	for (int j = 0; j < ny; j++) {
		threads[threadCount++] = thread(calculatePixelRow, j, cam, world, j*nx * 3);
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