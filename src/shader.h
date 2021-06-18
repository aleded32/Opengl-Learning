#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "glew.h"
#include <sstream>
#include <unordered_map>
#include <iostream>
#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"

class shader 
{
private:

	unsigned int m_RendererID;
	std::string vertexShader;
	std::string fragmentShader;
	std::unordered_map<std::string, unsigned int> m_uniformLocationCache;


	unsigned int m_getUniformLocation(const char* uniformName) 
	{
		unsigned int location = glGetUniformLocation(m_RendererID, uniformName);

		if (m_uniformLocationCache.find(uniformName) != m_uniformLocationCache.end()) 
		{
			return m_uniformLocationCache[uniformName];
		}

		if (glGetUniformLocation(m_RendererID, uniformName) != -1) 
		{
			m_uniformLocationCache[uniformName] = location;
			return location;
		}
		else 
		{
			std::cout << "warning uniform" <<  uniformName << " doesn't exist" << std::endl;
 		}
	}

	static unsigned int compileShader(const std::string& source, unsigned int type);
	static unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader);

public:

	shader(const std::string& filePath);
	~shader();

	void setUniform4f(const std::string& uniformName, float v0, float v1, float v2, float v3) 
	{
		unsigned int location = m_getUniformLocation(uniformName.c_str());
		glUniform4f(location, v0, v1, v2, v3);
	}

	void setUniformMat4f(const std::string& uniformName, const glm::mat4& matrix) 
	{
		unsigned int location = m_getUniformLocation(uniformName.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]);
	}

	


	void setUniform1i(const std::string& uniformName, int v0)
	{
		unsigned int location = m_getUniformLocation(uniformName.c_str());
		glUniform1i(location, v0);
	}

	void bind() const;
	void unbind() const;

};
