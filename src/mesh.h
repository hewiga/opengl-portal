#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include <vector>

#include "shader.h"
#include "OBJ_Loader.h"
#include "textures.h"

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoord;
};


class Mesh {

	unsigned int m_textureID;

protected:
	unsigned int m_vao, m_vbo, m_ebo;
	void prepareMesh();

public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	Mesh();
	Mesh(objl::Mesh& meshData, Textures& m_textures);
	void draw(Shader &shader);
};