#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>

#include <map>
#include <iostream>

#include "stb_image.h"

class Textures {

	std::map<std::string, unsigned int> m_textures;
	std::string m_lastTexture;
	unsigned int loadTexture(std::string texSrc);

public:
	Textures();
	unsigned int getTexture(std::string texSrc);
};