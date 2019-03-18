#include <vector>

#include "scene_conchoid.h"

#include "vec2.h"

using cgmath::vec2;

float POINT_DISTANCE = 0.01f;

void scene_conchoid::sendVertexData() {
	for (int i = 0; i < 2; i++) {
		updateVBO(vaos[i], positionsVBOs[i], 0, points[i], 2, GL_FLOAT, GL_DYNAMIC_DRAW);
	}
}

void  scene_conchoid::conchoid() {

	points[0].clear();
	points[1].clear();
	
	vec2 c = end - start;
	vec2 currentPoint;
	vec2 toOrigin;

	float magnitude = c.magnitude();
	c.normalize();

	for (float i = 0; i < magnitude; i += POINT_DISTANCE) {
		currentPoint = start + (c * i);

		toOrigin = origin - currentPoint;
		toOrigin.normalize();

		points[0].push_back(currentPoint + toOrigin * distance);
		//points[1].push_back(currentPoint);
		points[1].push_back(currentPoint - toOrigin * distance);
	}
}

void scene_conchoid::init() {

	conchoid();

	// Crear un identificador para un 
	// Vertex Array Object
	// Guarda el id en vao
	glGenVertexArrays(2, vaos);

	// Crear un identificador para un
	// Vertex Buffer Object
	// Guarda el id en positionsVBO
	glGenBuffers(2, positionsVBOs);

	sendVertexData();
}

void scene_conchoid::awake()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void scene_conchoid::mainLoop() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	for (int i = 0; i < 2; i++) {
		// Bind del vao que tiene todos los atributos
		glBindVertexArray(vaos[i]);

		// Llamada a dibujar
		// Tipo de primitiva
		// Desde que vertice se empieza a dibujar
		// Cuantos vertices se dibujan
		glDrawArrays(GL_LINE_STRIP, 0, points[i].size());
	}
	// Unbind del vao y todos los atributos
	glBindVertexArray(0);
}


void scene_conchoid::normalKeysDown(unsigned char key) {
	if (key == 'c') {
		distance -= POINT_DISTANCE;
	} else if (key == 'v') {
		distance += POINT_DISTANCE;
	} else if (key == 'z') {
		start.x -= POINT_DISTANCE;
		end.x += POINT_DISTANCE;
	} else if (key == 'x') {
		start.x += POINT_DISTANCE;
		end.x -= POINT_DISTANCE;
	}

	conchoid();
	sendVertexData();
}