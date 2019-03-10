#include "scene_primitives.h"

#include <vector>

#include "vec2.h"

// Se manda a llamar una vez cuando inicia la app.
void scene_primitives::init() {
	std::vector<cgmath::vec2> positions;

	positions.push_back(cgmath::vec2(0.0f, 0.0f));
	positions.push_back(cgmath::vec2(-1.0f, 0.0f));
	positions.push_back(cgmath::vec2(0.0f, -0.4f));
	positions.push_back(cgmath::vec2(1.0f, 0.0f));

	// Crear un identificador para un 
	// Vertex Array Object
	// Guarda el id en vao
	glGenVertexArrays(1, &vao);
	// Quiero empezar a trabajar con 
	// el siguiente vao
	glBindVertexArray(vao);

	// Crear un identificador para un
	// Vertex Buffer Object
	// Guarda el id en positionsVBO
	glGenBuffers(1, &positionsVBO);
	// Quiero trabajar con el buffer 
	// positionsVBO
	glBindBuffer(GL_ARRAY_BUFFER, positionsVBO);

	// Crea la memoria del buffer, 
	// especifica los datos y la
	// manda al GPU.
	glBufferData(GL_ARRAY_BUFFER,
		positions.size() * sizeof(cgmath::vec2),
		positions.data(),
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

	primitiveType = GL_POINTS;
}

void scene_primitives::awake()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glPointSize(20.0f);
}

void scene_primitives::sleep()
{
	glPointSize(1.0f);
}

void scene_primitives::mainLoop()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Bind del vao que tiene todos los atributos
	glBindVertexArray(vao);

	// Llamada a dibujar
	// Tipo de primitiva
	// Desde que vertice se empieza a dibujar
	// Cuantos vertices se dibujan
	glDrawArrays(primitiveType, 0, 4);

	// Unbind del vao y todos los atributos
	glBindVertexArray(0);
}

void scene_primitives::normalKeysDown(unsigned char key)
{
	switch (key) {
	case '1':
		primitiveType = GL_POINTS;
		break;
	case '2':
		primitiveType = GL_LINES;
		break;
	case '3':
		primitiveType = GL_LINE_STRIP;
		break;
	case '4':
		primitiveType = GL_LINE_LOOP;
		break;
	case '5':
		primitiveType = GL_TRIANGLES;
		break;
	case '6':
		primitiveType = GL_TRIANGLE_STRIP;
		break;
	case '7':
		primitiveType = GL_TRIANGLE_FAN;
		break;
	}
}