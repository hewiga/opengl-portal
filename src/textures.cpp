#include "textures.h"

Textures::Textures() { }

unsigned int Textures::loadTexture(std::string texSrc) {


	unsigned int texID;
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, channels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(texSrc.c_str(), &width, &height, &channels, 0);
	if (data == NULL) {
		std::cout << "Couldn't load texture " << texSrc << "!" << std::endl;
		return 0;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	return texID;
}

unsigned int Textures::getTexture(std::string texSrc) {

	if (texSrc == "") {
		return m_textures.find(m_lastTexture)->second;
	}
	m_lastTexture = texSrc;

	//check if texture isnt already loaded
	auto search = m_textures.find(texSrc);
	
	//if is return texture ID
	if (search != m_textures.end()) {
		return search->second;
	}

	//if not, load this texture and return its ID
	auto result = m_textures.insert({ texSrc, loadTexture(texSrc) });
	return m_textures.find(texSrc)->second;
}