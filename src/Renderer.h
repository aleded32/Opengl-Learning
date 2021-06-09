#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <glew.h>
#include <glfw3.h>
#include <Iostream>
#include <sstream>
#include "stb_image.h"

//this is macro that allows us to break the debugger by asserting itself when the error boolean is true. 
 //debugbreak is visual studio compiler specific; 
//this should be wrapped around all GL functions;
#define call(x) x;\
	if(error) __debugbreak();

#define ASSERT(x) if(!(x)) __debugbreak();\

class renderer 
{
private:
	int m_width;
	int m_height;
	const char* m_name;
	GLFWwindow* window;



public:

	GLFWwindow* getWindow() 
	{
		return window;
	}

	renderer(const int& width, const int& height, const char* name)
		: m_width(width), m_height(height), m_name(name) {}


	int createWindow();
	~renderer() {}
	
};