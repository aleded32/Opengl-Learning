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

struct shaderSourceProgram 
{
	std::string vertexSource;
	std::string fragmentSource;
};

//textures
unsigned char* m_localBuffer;
int m_width, m_height, m_bitsPerPixel;
unsigned int m_rendererID;
std::string m_filepath;



void textureCreate(const std::string& path)
{
	m_rendererID = 0;
	m_localBuffer = nullptr;
	m_width = 0;
	m_height = 0;
	m_bitsPerPixel = 0;
	m_filepath = path;

	//need to flip image as opengl reads pixels from the bottom left not top left. 
	stbi_set_flip_vertically_on_load(1);
	//sets the local buffer of the data of png file we are loading into the buffer.
	//channels in file is the bits per pixel and the desired channels is teh colour so four for rgba
	m_localBuffer = stbi_load(path.c_str(), &m_width, &m_height, &m_bitsPerPixel, 4);


	//generate a texture slot
	glGenTextures(1, &m_rendererID);
	//will bind the texture to the renderer id, with texture 2d
	glBindTexture(GL_TEXTURE_2D, m_rendererID);

	//all the parameters needed to display our texture, refer to documentation for different parameter types
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_localBuffer);

	
	if (m_localBuffer)
		stbi_image_free(m_localBuffer);



}

void destructorTex() 
{
	glDeleteTextures(1, &m_rendererID);
}

//if parameter = something, means its an optional parameter that will default to something if not used
void bind(unsigned int slot = 0) 
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_rendererID);

}
void unbind() 
{
	
}



//end of texture code
bool error = false;


//new version of getting errors this needs to be set in order to call and recieve debug info.
void APIENTRY messageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) 
{
	std::cout << source << "\n" << type << "\n" << message << std::endl;
	error = true;
}

static shaderSourceProgram ParseShader(const std::string& filepath) 
{
	FILE* fp;
	
	fp = fopen(filepath.c_str(), "r+");

	if (!fp)
	{
		std::cout << "file cannot be read" << std::endl;
		return { NULL, NULL };
	}
	char line[128];
	char buf[128];


	std::stringstream ss[2];

	enum class shaderType 
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	shaderType type = shaderType::NONE;

	while (fgets(line, sizeof(line), fp) != NULL) 
	{
		
		if(line[8] == *"v" && line[9] == *"e" && line[10] == *"r")
		{
			type = shaderType::VERTEX;
		}
		else if (line[8] == *"f" && line[9] == *"r" && line[10] == *"a")
		{
			type = shaderType::FRAGMENT;
		}
		else 
		{
			
			if (type == shaderType::VERTEX)
			{
			
				ss[(int)type] << line;
				
				
			}
			else if (type == shaderType::FRAGMENT) 
			{
				ss[(int)type] << line;
				
			}
		}
	}
	
	return{ ss[(int)shaderType::VERTEX].str(), ss[(int)shaderType::FRAGMENT].str() };

}

//grabs the shader and compiles it.
static unsigned int complileShader(const std::string& source, unsigned int type) 
{
	//creates shader id
	unsigned int id = glCreateShader(type);
	//sets the shader source
	const char* src = source.c_str();

	if (src != nullptr)
	{
		glShaderSource(id, 1, &src, nullptr);
		glCompileShader(id);


		//error handling for shader compilation
		int result;
		glGetShaderiv(id, GL_COMPILE_STATUS, &result);
		if (result == GL_FALSE) 
		{
			int length;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
			//allocates to the stack dynamically, instead of heap and using char pointer that you would later delete. 
			char* message = (char*)alloca(length * sizeof(char));
			glGetShaderInfoLog(id, length, &length, message);
			std::cout << "Failed to compile shader " << std::endl;
			std::cout << message << std::endl;
			glDeleteShader(id);
			return 0;

		}

		return id;
	}
	else 
	{
		std::cout << "could not grab source of shader";
		return 0;
	}
}

//creates the complete shader for us to compile. 
static unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader) 
{
	unsigned int program = glCreateProgram();
	unsigned int vs = complileShader(vertexShader, GL_VERTEX_SHADER);
	unsigned int fs = complileShader(fragmentShader, GL_FRAGMENT_SHADER);

	// this attachs the shader to a program, we then link program and validate it. Finally delete the shader has its already been linked
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	//not completely necessary as it makes debugging shader code near impossible and not deleting it only causes very minor
	//performance setbacks
	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

int main() 
{
	
	GLFWwindow* window;
	
	//unsigned int  = GLuint
	


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

	window = glfwCreateWindow(640, 480, "Test Area", NULL, NULL);

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

	glEnable(GL_DEBUG_OUTPUT); //enables debug output
	glDebugMessageCallback(messageCallback, 0); // produces and calls back the error that was produced. 

	//contains data about the vertex
	float vertexPos[] =
	{//  /vertex    / TexCoord  /
		-0.5f, -0.5f, 0.0f, 0.0f,
		 0.5f, -0.5f, 1.0f, 0.0f,
		 0.5f, 0.5f,  1.0f, 1.0f,
		-0.5, 0.5f,   0.0, 1.0f,

	};

	//these indicies are used to prevent use double the buffer for one vertex, by illustrating what point goes where
	unsigned int indicies[] =
	{
		0,1,2,
		2,3,0
	};

	//vertex array object, this needs to be created in order to bind buffers and specify layouts. 
	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Buffer to store data for vertices of shape. Static doesn't allow the change of vertices and is accessed many times.
	//dynamic allows new data in the buffer and used many times. 
	
	//required to store address of data
	unsigned int buffer;
	//generates the data
	glGenBuffers(1, &buffer);
	//binds it to an actual array buffer
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	//inputs the data for buffer, specifies the buffer used, size of the buffer, the data itself(vertices) and how its being drawn
	glBufferData(GL_ARRAY_BUFFER, 4 * 4 * sizeof(float), vertexPos, GL_STATIC_DRAW);


	

	//attributes are different components of the vertex (positon, colour etc) 
	// use documentation for rest of info.
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0); // this code specifiys which vertex array buffer to use with an index of zero binding the vertex buffer to the vertex array
	//NEEDED TO ENABLE THE ATTRIBUTE!
	glEnableVertexAttribArray(0);

	 glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (const void*) 8); // this code specifiys which vertex array buffer to use with an index of zero binding the vertex buffer to the vertex array
	//NEEDED TO ENABLE THE ATTRIBUTE!
	glEnableVertexAttribArray(1);

	unsigned int ibo; //index buffer object
	glGenBuffers(1, &ibo);
	//this uses gl elemetn array buffer as we are binding the incides the vertices will use.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indicies, GL_STATIC_DRAW);

	//shader will be read from file
	shaderSourceProgram source = ParseShader("res/shaders/basic.shader");
	/*std::cout << "vertex" << std::endl;
	std::cout << source.vertexSource << std::endl;
	std::cout << "fragment" << std::endl;
	std::cout << source.fragmentSource << std::endl;
	*/

	//creates the shader
	unsigned int shader = createShader(source.vertexSource, source.fragmentSource);
	//uses the shader
	glUseProgram(shader);

	//uniforms allow us put data into a shader, before a drawing of the object starts
	//first value is the location of the shader, which is set in the shader
	/*int location = glGetUniformLocation(shader, "u_color");
	ASSERT(location != -1);*/
	float r = 0;
	float increment = 0.05f;


	//call our texture methods
	textureCreate("res/textures/knight.png");
	bind(0);
	//shader needs to sample the texture in order to render it to the screen. the last param is the slot of the binded texture
	ASSERT(glGetUniformLocation(shader, "u_Texture") != -1);
	glUniform1i(glGetUniformLocation(shader, "u_Texture"), 0);


	//this will unbind everything before rebinding in loop
	glUseProgram(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		//rebound at runtime
		glUseProgram(shader);
		//glUniform4f(location, r, 0.8, 0.1, 1.0);
		
		//std::cout << glGetUniformLocation(shader, "u_Texture") << std::endl;
		
		//we can remove the arrtibpointer and attribarray from runtime and the bind buffer, just with binding the vao to get the same outcome
		//we are linking the buffer to the vertex array in the vertexarribpointer.
		glBindVertexArray(vao);
		
		/* this is how it would be used if we only use one vertext array object could potenially be faster so use this way in other programs
		   do not clear the vertex array object if using this way. 
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
		glEnableVertexAttribArray(0);
		*/
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		

		//count = number of indices
		
		call(glDrawElements(GL_TRIANGLES, sizeof(indicies), GL_UNSIGNED_INT, nullptr);)
		
			//just to show it changing the throug colours and demonstrate the use of uniforms.
			/*if (r < 0) 
			{
				increment = 0.05f;
			}
			else if (r > 1.0f) 
			{
				increment = -increment;
			}

		r += increment;
		*/
		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();

		
	}

	glDeleteProgram(shader);

	glfwTerminate();
	return 0;
}