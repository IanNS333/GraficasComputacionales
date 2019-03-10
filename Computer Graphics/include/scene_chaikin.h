#pragma once

#include <vector>
#include <unordered_set>
#include "scene.h"
#include "vec2.h"

using cgmath::vec2;

class scene_chaikin : public scene
{
public:
	void init();
	void awake();
	void sleep() {}
	void reset() {}
	void mainLoop();
	void resize(int width, int height) {}
	void normalKeysDown(unsigned char key);
	void normalKeysUp(unsigned char key) {}
	void specialKeys(int key) {}
	void passiveMotion(int x, int y) {}

	std::vector<vec2> createDottedLines(std::vector<vec2> points);
	std::vector<vec2> chaikin(std::vector<vec2> points, int iterations, bool isClosed);
	void sendVertexData();
	void sendDottedData();

private:

	static const int segmentCount = 27;

	bool showChaikin = true;
	bool showDotted = true;

	// Este es el manager de atributos
	GLuint vaos[segmentCount];
	GLuint dottedVaos[segmentCount];
	// Este es el buffer con el atributo
	GLuint positionsVBOs[segmentCount];
	GLuint dottedVBOs[segmentCount];

	std::vector<std::vector<vec2>> segments = std::vector<std::vector<vec2>>(segmentCount);
	std::vector<std::vector<vec2>> dottedSegments = std::vector<std::vector<vec2>>(segmentCount);

	std::unordered_set<int> closedSegments = {26};

	// https://www.geogebra.org/graphing/hqjeua9s
	std::vector<std::vector<vec2>> originalSegments = {
		{ {-4.84, 10.07}, {-4.87, 11.39}, {-3.55, 13.18}, {0.26, 12.88} , {3.38, 9.25}, {4.25, 6.80} },
		{ {3.25, 5.59}, {0.68, 9.19}, {-5.95, 10.24}, {-8, 9}, {-5.65, 9.21}, {-3.31, 8.13}, {0.28, 6.45}, {1.95, 7.26} },
		{ {4, 7.5}, {5.25, 7.21},{6.09, 5.85}, {5.63, 4.89}},
		{ {3.25, 5.59}, {4.73, 5.89}, {6.42, 3.97}, {5.00, 1.31}, {2.22, 1.83}},
		{ {4.13, 4.47}, {3.01, 4.01}, {2.93, 2.62}, {4.67, 2.65}},
/*5*/	{ {3.25, 5.59}, {1.96, 3.30}, {0.87, 3.43}, {0.86, 4.07}},
		{ {0.86, 4.07}, {0.27, 4.43}, {0.39, 4.97}, {1.51, 5.60}, {1.95, 7.26}},
		{ {-3.6, 5.16}, {-4.49, 6.22}, {-7.07, 6.17}, {-7.84, 4.04}, {-6.59, 2.60}, {-4.20, 2.51}, {-3.40, 3.43} },
		{ {-4.36, 8.50}, {-4.97, 7.94}, {-5.32, 7.02}, {-5.33, 6.18}},
		{ {-3.38, 3.45}, {-2.77, 3.46}, {-1.69, 4.06}, {-0.96,4.59}, {-0.54, 4.51}, {-0.66, 2.67}, {-1.60, 2.50}},
/*10*/	{ {-1.60, 2.50}, {-1.53, 1.66}, {-2.45, 1.16}, {-3.02, 1.51}},
		{ {-4.48, 1.47}, {-4.08, 1.11}, {-3.29, 1.14}, {-3.02, 1.51}},
		{ {-5.61, 2.55}, {-5.62, 1.63}, {-4.97, 1.12}, {-4.48, 1.47}},
		{ {-3.72, 1.13}, {-2.22, 0.40}, {1.29, 0.83}, {2.22, 1.83}},
		{ {-4.56, 10.08}, {-4.68, 10.52}, {-4.42, 11.55}, {-3.47, 12.40}, {-2.04, 12.28}, {-1.09, 11.27}, {-1.07, 9.69}, {-1.32, 9.47}},
/*15*/	{ {-4.18, 10.19}, {-3.39, 11.73}},
		{ {-3.39, 11.73}, {-2.94, 10.85}},
		{ {-2.94, 10.85}, {-2.27, 11.54}},
		{ {-2.27, 11.54}, {-1.95, 9.83}},
		{ {-1.95, 9.83}, {-2.52, 9.66}},
/*20*/	{ {-2.52, 9.66}, {-2.55, 10.56}},
		{ {-2.55, 10.56}, {-3.07, 10.04}},
		{ {-3.07, 10.04}, {-3.41, 10.71}},
		{ {-3.41, 10.71}, {-3.72, 9.89}},
		{ {-3.72, 9.89}, {-4.18, 10.19}},
/*25*/	{ {-5.03, 6.13}, {-5.09, 6.79}, {-4.82, 7.70}, {-4.19, 7.63}, {-4.20, 6.17}, {-4.36, 5.93}},
		{ {-2.56, 4.98}, {-2.31, 7.19}, {-1.45, 7.07}, {-1.70, 4.86}, {-2.56, 4.98}}
	};

};
