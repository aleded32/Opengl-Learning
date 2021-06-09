#include "vertexbuffer.h"

vertexBuffer::vertexBuffer(unsigned int size, const void* data)
{
	glGenBuffers(1, &m_Renderer_ID);
	glBindBuffer(GL_ARRAY_BUFFER, m_Renderer_ID);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void vertexBuffer::bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_Renderer_ID);
}

void vertexBuffer::unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

vertexBuffer::~vertexBuffer() 
{
	glDeleteBuffers(1, &m_Renderer_ID);
}