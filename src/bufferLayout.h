#pragma once
#include <vector>

class bufferLayout 
{
private:
	
	struct bufferLayoutElements
	{
		unsigned int count;
		unsigned int type;
		bool normalized;
	};

	unsigned int m_stride;
	
	unsigned int m_RendererID;

	std::vector<bufferLayoutElements> elements;

public:
	
	bufferLayout() 
		: m_stride(0) {}

	~bufferLayout() {}

	void addFloat(unsigned int count) 
	{
		elements.push_back({ count, GL_FLOAT, false });
		m_stride += sizeof(float) * count;
	}

	void addUint(unsigned int count)
	{
		elements.push_back({ count, GL_UNSIGNED_INT, false });
		m_stride += sizeof(unsigned int) * count;
	}

	

	void addUChar(unsigned int count)
	{
		elements.push_back({ count, GL_UNSIGNED_BYTE, true});
		m_stride += sizeof(char) * count;
	}

	void addShort(unsigned int count)
	{
		elements.push_back({ count, GL_SHORT, false });
		m_stride += sizeof(short) * count;
	}
	
	inline std::vector<bufferLayoutElements> getElements() const& { return elements; }; 
	inline unsigned int getStride() const& { return m_stride; }; 

};