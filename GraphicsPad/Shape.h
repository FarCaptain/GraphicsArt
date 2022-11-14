#pragma once
#include <GL/glew.h>
#include <Vector.h>

struct Shape {
	Shape() : vertices(0), numVertices(0), indices(0), numIndices(0) {}
	/*~Shape() {
		cleanup();
	}*/

	Vertex3D* vertices;
	GLuint numVertices;

	GLushort* indices;
	GLuint numIndices;

	GLsizeiptr vertexBufferSize() const {
		return numVertices * sizeof(Vertex3D);
	}

	GLsizeiptr indexBufferSize() const {
		return numIndices * sizeof(GLushort);
	}

	GLsizeiptr fullBufferSize() const {
		return vertexBufferSize() + indexBufferSize();
	}

	void cleanup() {
		delete[] vertices;
		delete[] indices;
		numVertices = numIndices = 0;
	}
};