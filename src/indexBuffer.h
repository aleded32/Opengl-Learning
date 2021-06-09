#pragma once
#include "Renderer.h"

class indexBuffer 
{
private:
	unsigned int m_Renderer_ID;
	unsigned int m_count;

public:

	indexBuffer(const void* data, unsigned int count);
	~indexBuffer();

	void bind() const;

	void unbind() const;
};