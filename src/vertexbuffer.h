#pragma once
#include <glew.h>

class vertexBuffer 
{
private:
	unsigned int m_Renderer_ID;

public:

	vertexBuffer(unsigned int size, const void* data);
	~vertexBuffer();

	void bind() const; 

	void unbind() const;
};