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
		{ {-4.84f, 10.07f}, {-4.87f, 11.39f}, {-3.55f, 13.18f}, {0.26f, 12.88f}, {3.38f, 9.25f}, {4.25f, 6.80f} },
		{ {3.25f, 5.59f}, {0.68f, 9.19f}, {-5.95f, 10.24f}, {-8.0f, 9.0f}, {-5.65f, 9.21f}, {-3.31f, 8.13f}, {0.28f, 6.45f}, {1.95f, 7.26f}},
		{ {4.0f, 7.5f}, {5.25f, 7.21f},{6.09f, 5.85f}, {5.63f, 4.89f}},
		{ {3.25f, 5.59f}, {4.73f, 5.89f}, {6.42f, 3.97f}, {5.00f, 1.31f}, {2.22f, 1.83f}},
		{ {4.13f, 4.47f}, {3.01f, 4.01f}, {2.93f, 2.62f}, {4.67f, 2.65f}},
/*5*/	{ {3.25f, 5.59f}, {1.96f, 3.30f}, {0.87f, 3.43f}, {0.86f, 4.07f}},
		{ {0.86f, 4.07f}, {0.27f, 4.43f}, {0.39f, 4.97f}, {1.51f, 5.60f}, {1.95f, 7.26f}},
		{ {-3.6f, 5.16f}, {-4.49f, 6.22f}, {-7.07f, 6.17f}, {-7.84f, 4.04f}, {-6.59f, 2.60f}, {-4.20f, 2.51f}, {-3.40f, 3.43f}},
		{ {-4.36f, 8.50f}, {-4.97f, 7.94f}, {-5.32f, 7.02f}, {-5.33f, 6.18f}},
		{ {-3.38f, 3.45f}, {-2.77f, 3.46f}, {-1.69f, 4.06f}, {-0.96f,4.59f}, {-0.54f, 4.51f}, {-0.66f, 2.67f}, {-1.60f, 2.50f}},
/*10*/	{ {-1.60f, 2.50f}, {-1.53f, 1.66f}, {-2.45f, 1.16f}, {-3.02f, 1.51f}},
		{ {-4.48f, 1.47f}, {-4.08f, 1.11f}, {-3.29f, 1.14f}, {-3.02f, 1.51f}},
		{ {-5.61f, 2.55f}, {-5.62f, 1.63f}, {-4.97f, 1.12f}, {-4.48f, 1.47f}},
		{ {-3.72f, 1.13f}, {-2.22f, 0.40f}, {1.29f, 0.83f}, {2.22f, 1.83f}},
		{ {-4.56f, 10.08f}, {-4.68f, 10.52f}, {-4.42f, 11.55f}, {-3.47f, 12.40f}, {-2.04f, 12.28f}, {-1.09f, 11.27f}, {-1.07f, 9.69f}, {-1.32f, 9.47f}},
/*15*/	{ {-4.18f, 10.19f}, {-3.39f, 11.73f}},
		{ {-3.39f, 11.73f}, {-2.94f, 10.85f}},
		{ {-2.94f, 10.85f}, {-2.27f, 11.54f}},
		{ {-2.27f, 11.54f}, {-1.95f, 9.83f}},
		{ {-1.95f, 9.83f}, {-2.52f, 9.66f}},
/*20*/	{ {-2.52f, 9.66f}, {-2.55f, 10.56f}},
		{ {-2.55f, 10.56f}, {-3.07f, 10.04f}},
		{ {-3.07f, 10.04f}, {-3.41f, 10.71f}},
		{ {-3.41f, 10.71f}, {-3.72f, 9.89f}},
		{ {-3.72f, 9.89f}, {-4.18f, 10.19f}},
/*25*/	{ {-5.03f, 6.13f}, {-5.09f, 6.79f}, {-4.82f, 7.70f}, {-4.19f, 7.63f}, {-4.20f, 6.17f}, {-4.36f, 5.93f}},
		{ {-2.56f, 4.98f}, {-2.31f, 7.19f}, {-1.45f, 7.07f}, {-1.70f, 4.86f}, {-2.56f, 4.98f}}
	};

};
