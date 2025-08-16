#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include <vector>
#include <math.h>

#define _USE_MATH_DEFINES

#include "shader.h"
#include "OBJ_Loader.h"
#include "mesh.h"

class Portal : public Mesh{

	Portal* m_destinationPortal;
	glm::mat4 m_model;
	glm::vec3 m_rotationAxis;
	glm::vec3 m_position;
	float m_angle;

	void setPositionFromVertices(std::vector<objl::Vertex>& v);
	void setRotationFromNormals(objl::Vector3 normals);

public:
	Portal(objl::Mesh& meshData);
	
	Portal* getDestinationPortal();
	void connectWithPortal(Portal* destinationPortal);

	std::vector<Vertex> getVertices();
	glm::mat4 getModel();
	glm::mat4 portalView(glm::mat4 cameraView);
	glm::vec3 getRotationAxis();
	glm::vec3 getPosition();

	float getAngle();
	void draw(Shader& shader);
};