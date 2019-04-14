#pragma once

#include <vector>
#include "vec2.h"
#include "utils.h"

using std::vector;
using cgmath::vec2;
using utils::randf;

namespace perlin {

	vector<vector<vec2>> gradients;


	float linear_interpolation(float a0, float a1, float w) {
		return (1.0f - w)*a0 + w * a1;
	}

	void  generate_grid(int width, int height) {

		for (int j = 0; j <= height; j++) {
			gradients.push_back(vector<vec2>());
			for (int i = 0; i <= width; i++) {
				gradients[j].push_back(vec2::normalize({ randf(), randf() }));
			}
		}
	}

	float height_at(float x, float y) {

		int x0 = (int) x;
		int x1 = x0 + 1;

		int y0 = (int) y;
		int y1 = y0 + 1;

		float dot1 = vec2::dot(vec2(x, y) - vec2(x0, y0), gradients[x0][y0]);
		float dot2 = vec2::dot(vec2(x, y) - vec2(x1, y0), gradients[x1][y0]);

		float dot3 = vec2::dot(vec2(x, y) - vec2(x0, y1), gradients[x0][y1]);
		float dot4 = vec2::dot(vec2(x, y) - vec2(x1, y1), gradients[x1][y1]);

		float res = linear_interpolation(
			linear_interpolation(dot1, dot2, x - x0),
			linear_interpolation(dot3, dot4, x - x0),
			y - y0
		);

		return res;
	}

};