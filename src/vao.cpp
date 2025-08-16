#include "vao.h"

VAO::VAO() {
	glGenVertexArrays(1, &m_id);
}

void VAO::linkAttrib(VBO& vbo, unsigned int layout, size_t layoutSize, GLenum type, size_t stride, void* pointer) {

	vbo.bindBuffer();
	glVertexAttribPointer(layout, layoutSize, type, GL_FALSE, stride, pointer);
	glEnableVertexAttribArray(layout);
	vbo.unbindBuffer();
}

void VAO::bindArray() {

	glBindVertexArray(m_id);
}

void VAO::unbindArray() {
	glBindVertexArray(0);
}

void VAO::deleteArray() {
	glDeleteVertexArrays(1, &m_id);
}
