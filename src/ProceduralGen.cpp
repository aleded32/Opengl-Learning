#include "vertexbuffer.h"
#include "indexBuffer.h"
#include "vertexArray.h"
#include "bufferLayout.h"
#include "texture.h"


struct shaderSourceProgram 
{
	std::string vertexSource;
	std::string fragmentSource;
};







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
	
	renderer Renderer(1280,720, "Test");
	Renderer.createWindow();
	

	

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

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	//vertex array object, this needs to be created in order to bind buffers and specify layouts. 
	vertexArray va;
	bufferLayout layout;
	layout.addFloat(2);
	layout.addFloat(2);

	
	

	/* Buffer to store data for vertices of shape. Static doesn't allow the change of vertices and is accessed many times.
	//dynamic allows new data in the buffer and used many times. 
	
	//required to store address of data
	unsigned int buffer;
	//generates the data
	glGenBuffers(1, &buffer);
	//binds it to an actual array buffer
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	//inputs the data for buffer, specifies the buffer used, size of the buffer, the data itself(vertices) and how its being drawn
	glBufferData(GL_ARRAY_BUFFER, 4 * 4 * sizeof(float), vertexPos, GL_STATIC_DRAW);
	*/



	vertexBuffer vb(4*4*sizeof(float), vertexPos);
	vb.bind();
	
	va.addBufferLayout(vb, layout);
	
	
	

	//attributes are different components of the vertex (positon, colour etc) 
	// use documentation for rest of info.
	//glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0); // this code specifiys which vertex array buffer to use with an index of zero binding the vertex buffer to the vertex array
	//NEEDED TO ENABLE THE ATTRIBUTE!
	//glEnableVertexAttribArray(0);

	 //glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (const void*) 8); // this code specifiys which vertex array buffer to use with an index of zero binding the vertex buffer to the vertex array
	//NEEDED TO ENABLE THE ATTRIBUTE!
	//glEnableVertexAttribArray(1);


	/*
	unsigned int ibo; //index buffer object
	glGenBuffers(1, &ibo);
	//this uses gl elemetn array buffer as we are binding the incides the vertices will use.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indicies, GL_STATIC_DRAW);
	*/

	indexBuffer ib(indicies, 6);
	ib.bind();



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
	texture tex("res/textures/knight.png");
	tex.bind(0);
	//shader needs to sample the texture in order to render it to the screen. the last param is the slot of the binded texture
	ASSERT(glGetUniformLocation(shader, "u_Texture") != -1);
	glUniform1i(glGetUniformLocation(shader, "u_Texture"), 0);


	//this will unbind everything before rebinding in loop
	glUseProgram(0);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	ib.unbind();
	vb.unbind();
	va.unbind();
	


	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(Renderer.getWindow()))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		//rebound at runtime
		glUseProgram(shader);
		//glUniform4f(location, r, 0.8, 0.1, 1.0);
		
		//std::cout << glGetUniformLocation(shader, "u_Texture") << std::endl;
		
		//we can remove the arrtibpointer and attribarray from runtime and the bind buffer, just with binding the vao to get the same outcome
		//we are linking the buffer to the vertex array in the vertexarribpointer.
		va.bind();
		/* this is how it would be used if we only use one vertext array object could potenially be faster so use this way in other programs
		   do not clear the vertex array object if using this way. 
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
		glEnableVertexAttribArray(0);
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		*/

		//count = number of indices
		
		call(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, indicies);)
		
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
		glfwSwapBuffers(Renderer.getWindow());

		/* Poll for and process events */
		glfwPollEvents();

		
	}

	glDeleteProgram(shader);

	glfwTerminate();
	return 0;
}