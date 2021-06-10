#include "texture.h"

texture::texture(const std::string& filePath)
	: m_localBuffer(nullptr), m_width(0), m_height(0), m_bitsPerPixel(0), m_filepath(filePath)
{
	//need to flip image as opengl reads pixels from the bottom left not top left. 
	stbi_set_flip_vertically_on_load(1);
	//sets the local buffer of the data of png file we are loading into the buffer.
	//channels in file is the bits per pixel and the desired channels is teh colour so four for rgba
	m_localBuffer = stbi_load(filePath.c_str(), &m_width, &m_height, &m_bitsPerPixel, 4);


	//generate a texture slot
	glGenTextures(1, &m_rendererID);
	//will bind the texture to the renderer id, with texture 2d
	glBindTexture(GL_TEXTURE_2D, m_rendererID);

	//all the parameters needed to display our texture, refer to documentation for different parameter types
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_localBuffer);


	if (m_localBuffer)
		stbi_image_free(m_localBuffer);

}

texture::~texture()
{
	glDeleteTextures(1, &m_rendererID);
}

//if parameter = something, means its an optional parameter that will default to something if not used
void texture::bind(unsigned int slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_rendererID);
}

void texture::unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
