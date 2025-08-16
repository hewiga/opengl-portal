#include "mesh.h"

Mesh::Mesh(objl::Mesh& meshData, Textures& m_textures) {

	for (int i = 0; i < meshData.Vertices.size(); i++) {
		objl::Vertex v = meshData.Vertices[i];
		vertices.push_back(Vertex{
				glm::vec3(v.Position.X, v.Position.Y, v.Position.Z),
				glm::vec3(v.Normal.X, v.Normal.Y, v.Normal.Z),
				glm::vec2(v.TextureCoordinate.X, v.TextureCoordinate.Y)
		});
	}
	indices = meshData.Indices;
	m_textureID = m_textures.getTexture(meshData.MeshMaterial.map_Kd);

	prepareMesh();
}

Mesh::Mesh() {}

void Mesh::prepareMesh() {

	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ebo);
	glGenVertexArrays(1, &m_vao);

	glBindVertexArray(m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	//vertices coords
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glEnableVertexAttribArray(0);
	
	//vertices normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(1);

	//texture coords
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}

void Mesh::draw(Shader &shader) {

	shader.activateShader();
	shader.setMat4("model", glm::mat4(1.0f));
	glBindVertexArray(m_vao);
	glBindTexture(GL_TEXTURE_2D, m_textureID);

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
