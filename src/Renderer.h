#pragma once
#include <glew.h>
#include <glfw3.h>
#include "indexBuffer.h"
#include "vertexArray.h"
#include "shader.h"

//this is macro that allows us to break the debugger by asserting itself when the error boolean is true. 
 //debugbreak is visual studio compiler specific; 
//this should be wrapped around all GL functions;
#define call(x) x;\
	if(Error) __debugbreak();

#define ASSERT(x) if(!(x)) __debugbreak();\

static bool Error = false;


//new version of getting errors this needs to be set in order to call and recieve debug info.
static void APIENTRY messageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar * message, const void* userParam)
{
	std::cout << source << "\n" << type << "\n" << message << std::endl;
	Error = true;
}

class renderer 
{
private:
	int m_width;
	int m_height;
	const char* m_name;
	GLFWwindow* window;



public:

	inline GLFWwindow* getWindow() 
	{
		return window;
	}

	inline const int getWidth() { return m_width; }
	inline const int getHeight() { return m_height; }
	
	renderer(const int& width, const int& height, const char* name)
		: m_width(width), m_height(height), m_name(name) {}

	void draw(const vertexArray& va, const indexBuffer& ib, const shader& Shader);
	void clear();

	int createWindow();
	~renderer() {}
	
};