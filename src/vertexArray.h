#pragma once
#include "vertexbuffer.h"
class bufferLayout;


class vertexArray 
{
private:
	unsigned int m_offset;
	unsigned int m_rendererID;

	inline const unsigned int getSizeOfType(unsigned int glType) const&
	{
		switch (glType)
		{
			case GL_FLOAT: 
				return 4;
				break;
			case GL_UNSIGNED_INT:
				return 4;
				break;
			case GL_SHORT: 
				return 2;
				break;
			case GL_UNSIGNED_BYTE: 
				return 1;
				break;
		}
	}

public:

	vertexArray();
	~vertexArray();


	const void bind() const;

	const void unbind() const;

	void addBufferLayout(const vertexBuffer& vb, const bufferLayout& layout);

};