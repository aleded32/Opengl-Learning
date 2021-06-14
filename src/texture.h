#pragma once
#include "stb_image.h"
#include "Renderer.h"

class texture 
{
private:

	
	unsigned char* m_localBuffer;
	int m_width, m_height, m_bitsPerPixel;
	unsigned int m_rendererID;
	std::string m_filepath;

public:

	texture(const std::string& filePath);
	~texture();

	void bind(unsigned int slot = 0) const;
	void unbind() const;
};