#include "portal.h"

Portal::Portal(objl::Mesh& meshData) :
	m_model(1.0f)
{

	for (int i = 0; i < meshData.Vertices.size(); i++) {
		objl::Vertex v = meshData.Vertices[i];
		vertices.push_back(Vertex{
				glm::vec3(v.Position.X, v.Position.Y, v.Position.Z),
				glm::vec3(v.Normal.X, v.Normal.Y, v.Normal.Z),
				glm::vec2(v.TextureCoordinate.X, v.TextureCoordinate.Y)
			});
	}
	indices = meshData.Indices;

	setPositionFromVertices(meshData.Vertices);
	setRotationFromNormals(meshData.Vertices[0].Normal);
	prepareMesh();

	m_model = glm::translate(m_model, m_position);
	if (m_angle != 0.0f) {
		m_model = glm::rotate(m_model, glm::radians(m_angle), m_rotationAxis);
	}

	m_destinationPortal = this;
}

Portal* Portal::getDestinationPortal() {
	return m_destinationPortal;
}

void Portal::setPositionFromVertices(std::vector<objl::Vertex>& v) {

	float minX = v[0].Position.X, maxX = v[0].Position.X;
	float minZ = v[0].Position.Z, maxZ = v[0].Position.Z;
	float minY = v[0].Position.Y;

	for (int i = 1; i < 4; i++) {
		objl::Vector3 position = v[i].Position;
		if (position.X > maxX) maxX = position.X;
		else if (position.X < minX) minX = position.X;

		if (position.Y < minY) minY = position.Y;

		if (position.Z > maxZ) maxZ = position.Z;
		else if (position.Z < minZ) minZ = position.Z;

	}
	m_position = glm::vec3(
		(maxX - minX) / 2 + minX,
		 minY,
		(maxZ - minZ) / 2 + minZ
	);
}

void Portal::setRotationFromNormals(objl::Vector3 normals) {

	if (normals.X > 0 && normals.Z == 0) m_angle = 90;
	else if (normals.X < 0 && normals.Z == 0) m_angle = 270;
	else if (normals.X == 0 && normals.Z > 0) m_angle = 0;
	else if (normals.X == 0 && normals.Z < 0) m_angle = 180;
	m_rotationAxis = glm::vec3(0.0f, 1.0f, 0.0f);
}

void Portal::connectWithPortal(Portal* portal) {

	m_destinationPortal = portal;
}

glm::mat4 Portal::portalView(glm::mat4 cameraView) {
	glm::mat4 mv = cameraView * m_model;

	glm::mat4 portalCam = 
		mv *
		glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0)) *
		glm::inverse(m_destinationPortal->getModel());

	return portalCam;
}

std::vector<Vertex> Portal::getVertices() {
	return vertices;
}

glm::mat4 Portal::getModel() {
	return m_model;
}

glm::vec3 Portal::getRotationAxis()
{
	return m_rotationAxis;
}

glm::vec3 Portal::getPosition()
{
	return m_position;
}

float Portal::getAngle()
{
	return m_angle;
}

void Portal::draw(Shader& shader) {
	
	shader.activateShader();
	shader.setMat4("model", glm::mat4(1.0f));
	glBindVertexArray(m_vao);

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}