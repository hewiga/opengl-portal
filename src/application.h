#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <gtx/string_cast.hpp>
#include <gtx/intersect.hpp>

#include <iostream>
#include <fstream>
#include <vector>

#include "textures.h"
#include "OBJ_Loader.h"
#include "shader.h"
#include "camera.h"
#include "portal.h"
#include "mesh.h"

class Application {

	const unsigned int WINDOWS_WIDTH;
	const unsigned int WINDOWS_HEIGHT;
	const float CAMERA_SPEED;
	
	bool m_firstMouseMove;

	float m_lastX;
	float m_lastY;

	const unsigned int MAX_PORTAL_DEPTH;

	Textures m_textures;
	Camera m_camera;
	std::vector<Mesh> m_meshes;
	std::vector<Portal> m_portals;

public:
	Application();
	void handleMouse(GLFWwindow* window);
	void handleInputs(GLFWwindow* window);
	void drawPortal(glm::mat4 view, Shader& shader, uint8_t portalDepth);
	int run();
};