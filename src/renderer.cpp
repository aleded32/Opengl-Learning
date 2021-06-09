#include "Renderer.h"


int renderer::createWindow()
{


	if (!glfwInit())
	{

		return -1;
	}

	//these 3 lines of code hints to glfw that we want to run in the core version of opengl 4, without this
	//it will run in compatability mode. we need this as we need to create our own vertex array;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE); // this allows the message to be seen in console

	window = glfwCreateWindow(m_width, m_height, m_name, NULL, NULL);

	if (!window)
	{
		glfwTerminate();
	}

	glfwMakeContextCurrent(window);

	//limits the frame rate to refresh rate of screen;
	glfwSwapInterval(1);

	GLenum err = glewInit();

	if (GLEW_OK != err)
		std::cout << glewGetErrorString(err);

	std::cout << glGetString(GL_VERSION) << std::endl;
}