#pragma once
#include<glew.h>

class indexBuffer 
{
private:
	unsigned int m_Renderer_ID;
	unsigned int m_count;

public:

	indexBuffer(const void* data, unsigned int count);
	~indexBuffer();

	const unsigned int getCount() const& 
	{
		return m_count;
	}

	void bind() const;

	void unbind() const;
};