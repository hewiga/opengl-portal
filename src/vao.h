#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "vbo.h"

class VAO {

	unsigned int m_id;

public:
	VAO();
	void linkAttrib(VBO& vbo, unsigned int layout, size_t layoutSize, GLenum type, size_t stride, void* pointer);
	void bindArray();
	void unbindArray();
	void deleteArray();
};