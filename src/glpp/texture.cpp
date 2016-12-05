#include "texture.h"

#include <stdexcept>

#include <SOIL/SOIL.h>

namespace glpp
{

Texture::Texture(GLuint texture)
	: m_handle(texture)
{}

Texture Texture::load1D(std::string const & path)
{
	GLuint textureID;
	glGenTextures(1, &textureID);

	int width, height;
	unsigned char* image = SOIL_load_image(path.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
	if (!image)
		throw std::runtime_error("failed to load texture from " + path);

	glBindTexture(GL_TEXTURE_1D, textureID);
	glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, width, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_1D);
	glBindTexture(GL_TEXTURE_1D, 0);

	SOIL_free_image_data(image);

	return Texture(textureID);
}

Texture::operator GLuint() const
{
	return m_handle;
}


}	// namespace glpp
