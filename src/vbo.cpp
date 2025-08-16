#include "vbo.h"

VBO::VBO(float* verticies, size_t size) {

	glGenBuffers(1, &m_id);
	glBindBuffer(GL_ARRAY_BUFFER, m_id);
	glBufferData(GL_ARRAY_BUFFER, size, verticies, GL_STATIC_DRAW);
}

VBO::VBO() {}

void VBO::setData(float* verticies, size_t size) {

	glGenBuffers(1, &m_id);
	glBindBuffer(GL_ARRAY_BUFFER, m_id);
	glBufferData(GL_ARRAY_BUFFER, size, verticies, GL_STATIC_DRAW);
}

void VBO::bindBuffer() {
	
	glBindBuffer(GL_ARRAY_BUFFER, m_id);
}

void VBO::unbindBuffer() {

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::deleteBuffer() {

	glDeleteBuffers(1, &m_id);
}

