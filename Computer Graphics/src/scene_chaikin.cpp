#include "scene_chaikin.h"

#include <vector>
#include "vec2.h"

float DOTTED_LINE_SIZE = 0.025f;

int min(int a, int b) {
	return a < b ? a : b;
}
int max(int a, int b) {
	return a > b ? a : b;
}

std::vector<vec2> scene_chaikin::createDottedLines(std::vector<vec2> points) {

	std::vector<vec2> result;
	vec2 a, b, c;
	float magnitude;
	for (unsigned int i = 0; i < points.size() - 1; i++) {
		a = points[i];
		b = points[i+1];
		c = b - a;
		magnitude = c.magnitude();
		c.normalize();

		result.push_back(a);
		for (float p = DOTTED_LINE_SIZE; p < magnitude; p += DOTTED_LINE_SIZE) {

			result.push_back(a + (c * p));
		}
		result.push_back(b);
	}

	return result;
}

std::vector<vec2> scene_chaikin::chaikin(std::vector<vec2> points, int iterations, bool isClosed = false) {
	vec2 one_fourth;
	vec2 three_fourths;
	vec2 a, b;
	std::vector<vec2> new_points = points;
	for (int i = 0; i < iterations; i++) {

		new_points.clear();
		if (!isClosed) {
			new_points.push_back(points[0]);
		}
		for (unsigned int i = 0; i < points.size() - 1; i++) {
			a = points[i];
			b = points[i + 1];
			one_fourth = a + (b - a) / 4.0f;
			three_fourths = a + (3.0f * (b - a)) / 4.0f;
			new_points.push_back(one_fourth);
			new_points.push_back(three_fourths);
		}
		if (!isClosed) {
			new_points.push_back(points[points.size() - 1]);
		} else {
			new_points.push_back(new_points[0]);
		}
		points = new_points;
	}
	return new_points;
}

void normalize(std::vector<std::vector<vec2>> &originalSegments) {
	int minimum = INT_MAX;
	int maximum = INT_MIN;

	int minX = INT_MAX;
	int minY = INT_MAX;

	int maxX= INT_MIN;
	int maxY = INT_MIN;

	for (unsigned int i = 0; i < originalSegments.size(); i++) {
		for (unsigned int j = 0; j < originalSegments[i].size(); j++) {

			minX = min(minX, (int)floorf(originalSegments[i][j].x));
			minY= min(minY, (int)floorf(originalSegments[i][j].y));

			maxX = max(maxX, (int)ceilf(originalSegments[i][j].x));
			maxY = max(maxY, (int)ceilf(originalSegments[i][j].y));
		}
	}

	vec2 origin((minX + maxX) / 2.0f, (minY + maxY) / 2.0f);

	for (unsigned int i = 0; i < originalSegments.size(); i++) {
		for (unsigned int j = 0; j < originalSegments[i].size(); j++) {

			originalSegments[i][j] -= origin;

			minimum = min(minimum, (int)floorf(originalSegments[i][j].x));
			minimum = min(minimum, (int)floorf(originalSegments[i][j].y));

			maximum = max(maximum, (int)ceilf(originalSegments[i][j].x));
			maximum = max(maximum, (int)ceilf(originalSegments[i][j].y));
		}
	}

	for (unsigned int i = 0; i < originalSegments.size(); i++) {
		for (unsigned int j = 0; j < originalSegments[i].size(); j++) {
			originalSegments[i][j].x = ((2 * (originalSegments[i][j].x - minimum)) / (maximum - minimum)) - 1;
			originalSegments[i][j].y = ((2 * (originalSegments[i][j].y - minimum)) / (maximum - minimum)) - 1;
		}
	}
}

void scene_chaikin::sendVertexData() {
	for (int i = 0; i < segmentCount; i++) {
		// Quiero empezar a trabajar con 
		// el siguiente vao
		glBindVertexArray(vaos[i]);

		// Quiero trabajar con el buffer 
		// positionsVBO
		glBindBuffer(GL_ARRAY_BUFFER, positionsVBOs[i]);

		// Crea la memoria del buffer, 
		// especifica los datos y la
		// manda al GPU.
		glBufferData(GL_ARRAY_BUFFER,
			segments[i].size() * sizeof(cgmath::vec2),
			segments[i].data(),
			GL_DYNAMIC_DRAW
		);

		// Prende el atributo 0
		glEnableVertexAttribArray(0);
		// Voy a configurar el atributo 0
		// Con 2 componentes
		// de tipo float
		// sin normalizar los datos
		// desfazamiento entre los atributos de la lista
		// Apuntador a los datos, si no los hemos mandado
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

		// Cuando haceos un bind con 0, 
		// en realidad significa unbind
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Unbind del vao
		glBindVertexArray(0);
	}
}

void scene_chaikin::sendDottedData() {
	for (int i = 0; i < segmentCount; i++) {
		updateVBO(dottedVaos[i], dottedVBOs[i], 0, dottedSegments[i], 2, GL_FLOAT, GL_DYNAMIC_DRAW);
	}
}

// Se manda a llamar una vez cuando inicia la app.
void scene_chaikin::init() {

	normalize(originalSegments);

	for (int i = 0; i < segmentCount; i++) {
		segments[i] = chaikin(originalSegments[i], 5, closedSegments.find(i) != closedSegments.end());
	}

	for (int i = 0; i < segmentCount; i++) {
		dottedSegments[i] = createDottedLines(originalSegments[i]);
	}



	// Crear un identificador para un 
	// Vertex Array Object
	// Guarda el id en vao
	glGenVertexArrays(segmentCount, vaos);
	glGenVertexArrays(segmentCount, dottedVaos);
	
	// Crear un identificador para un
	// Vertex Buffer Object
	// Guarda el id en positionsVBO
	glGenBuffers(segmentCount, positionsVBOs);
	glGenBuffers(segmentCount, dottedVBOs);

	sendVertexData();
	sendDottedData();
}

void scene_chaikin::awake()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void scene_chaikin::mainLoop()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (showChaikin) {
		for (int i = 0; i < segmentCount; i++) {
			// Bind del vao que tiene todos los atributos
			glBindVertexArray(vaos[i]);

			// Llamada a dibujar
			// Tipo de primitiva
			// Desde que vertice se empieza a dibujar
			// Cuantos vertices se dibujan
			glDrawArrays(GL_LINE_STRIP, 0, segments[i].size());
		}
	}

	if (showDotted) {
		for (int i = 0; i < segmentCount; i++) {
		// Bind del vao que tiene todos los atributos
			glBindVertexArray(dottedVaos[i]);

			// Llamada a dibujar
			// Tipo de primitiva
			// Desde que vertice se empieza a dibujar
			// Cuantos vertices se dibujan
			glDrawArrays(GL_LINES, 0, dottedSegments[i].size());
		}
	}

	// Unbind del vao y todos los atributos
	glBindVertexArray(0);
}

void scene_chaikin::normalKeysDown(unsigned char key)
{
	if ('0' <= key && key <= '9') {
		for (int i = 0; i < segmentCount; i++) {
			segments[i] = chaikin(originalSegments[i], key - '0', closedSegments.find(i) != closedSegments.end());
		}
	}
	if (key == 'c') {
		showChaikin = !showChaikin;
	}
	if (key == 'v') {
		showDotted = !showDotted;
	}
	sendVertexData();
}