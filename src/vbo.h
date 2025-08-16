#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class VBO {

	unsigned int m_id;

public:
	VBO();
	VBO(float* verticies, size_t size);
	void setData(float* verticies, size_t size);
	void bindBuffer();
	void unbindBuffer();
	void deleteBuffer();
};