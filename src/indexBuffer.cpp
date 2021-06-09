#include "indexBuffer.h"

indexBuffer::indexBuffer(const void* data, unsigned int count)
	: m_count(count)
{
	glGenBuffers(1, &m_Renderer_ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Renderer_ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_count * sizeof(unsigned int), data, GL_STATIC_DRAW);
}

//these are const as they have no modifiable values in the methods.
void indexBuffer::bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Renderer_ID);
}

void indexBuffer::unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

indexBuffer::~indexBuffer() 
{
	glDeleteBuffers(1, &m_Renderer_ID);
}

