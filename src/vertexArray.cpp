#include "vertexArray.h"

vertexArray::vertexArray()
	: m_offset(0)
{
	glGenVertexArrays(1, &m_rendererID);
	glBindVertexArray(m_rendererID);

}

vertexArray::~vertexArray()
{
	glDeleteVertexArrays(1, &m_rendererID);
}

const void vertexArray::bind() const
{
	glBindVertexArray(m_rendererID);
}

const void vertexArray::unbind() const
{
	glBindVertexArray(0);
}

void vertexArray::addBufferLayout(const vertexBuffer& vb, const bufferLayout& layout)
{
	vb.bind();
	bind();
	const auto& elements = layout.getElements();

	for (unsigned int i = 0; i < elements.size(); i++)
	{
		glVertexAttribPointer(i, elements[i].count, elements[i].type, elements[i].normalized ? GL_TRUE : GL_FALSE, layout.getStride(), (const void*)m_offset);
		glEnableVertexAttribArray(i);
		m_offset += (elements[i].count * getSizeOfType(elements[i].type));
		
	}
}
