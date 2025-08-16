#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>

class Shader {

	unsigned int m_id;

	std::string readShaderData(std::string shaderPath);
	void compileErrors(unsigned int shader, const char* type);

public:
	Shader(std::string vertexShaderPath, std::string fragmentShaderPath);
	void setMat4(std::string name, glm::mat4 value);
	void activateShader();
	void deleteShader();
};