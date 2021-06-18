#include "Renderer.h"
#include "vertexbuffer.h"
#include "bufferLayout.h"
#include "texture.h"
#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_glfw.h"
#include "vendor/imgui/imgui_impl_opengl3.h"


/*struct shaderSourceProgram 
{
	std::string vertexSource;
	std::string fragmentSource;
}*/







//end of texture code


/*static shaderSourceProgram ParseShader(const std::string& filepath) 
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

}*/

/*grabs the shader and compiles it.
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
}*/

/*creates the complete shader for us to compile. 
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
}*/

int main() 
{
	
	renderer Renderer(960,540, "Test");
	Renderer.createWindow();
	
	//initalises imgui;
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsLight();
	ImGui_ImplGlfw_InitForOpenGL(Renderer.getWindow(), true);
	ImGui_ImplOpenGL3_Init((char*)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));


	glEnable(GL_DEBUG_OUTPUT); //enables debug output
	glDebugMessageCallback(messageCallback, 0); // produces and calls back the error that was produced. 

	//contains data about the vertex
	float vertexPos[] =
	{//  /vertex    / TexCoord  /
		100.0f, 100.0f, 0.0f, 0.0f,
		 200.0f, 100.0f, 1.0f, 0.0f,
		 200.0f, 200.0f,  1.0f, 1.0f,
		100.0f, 200.0f,   0.0, 1.0f,

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

	//setting our model view porjection matrix. basically allows us to manipulate the screen space.
	//model = actual model position on screen.
	//view = viewport or camera matrix 
	//projection = position relativate to the space on screen, either 3D or 2D.
	
	//sets the projection.
	glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f,1.0f);

	//sets the view matrix by translating viewport 100pixels to the left and shifiting other geometry to the right in this instance.
	glm::mat4 view= glm::translate(glm::mat4(1.0), glm::vec3(-100, 0, 0));

	float angle = 180 * (3.141592 / 180);
	//sets the translation/rotation/scale for the model object itself. 
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(250, 250, 0));
	//model = glm::rotate(model, angle , glm::vec3(0,0, 1));
	//model = glm::scale(model, glm::vec3(0.5f,0.5f,0));

	//in other graphics api it is mvp not pvm like opengl due to being column abiding matrix reading instead of row
	glm::mat4 mvp = proj * view * model;


	shader Shader("res/Shaders/basic.shader");
	//Shader.setUniformMat4f("u_MVP", mvp);
	//shader will be read from file
	//shaderSourceProgram source = ParseShader("res/shaders/basic.shader");
	/*std::cout << "vertex" << std::endl;
	std::cout << source.vertexSource << std::endl;
	std::cout << "fragment" << std::endl;
	std::cout << source.fragmentSource << std::endl;
	*/

	//creates the shader
	//unsigned int shader = createShader(source.vertexSource, source.fragmentSource);
	//uses the shader
	//glUseProgram(shader);

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
	//ASSERT(glGetUniformLocation(shader, "u_Texture") != -1);
	//glUniform1i(glGetUniformLocation(shader, "u_Texture"), 0);
	Shader.setUniform1i("u_Texture", 0);
	

	//this will unbind everything before rebinding in loop
	//glUseProgram(0);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	Shader.unbind();
	ib.unbind();
	vb.unbind();
	va.unbind();
	
	glm::vec3 translation = glm::vec3(0, 0, 0);
	bool tabOpen = true;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(Renderer.getWindow()))
	{
		/* Render here */
		//glClear(GL_COLOR_BUFFER_BIT);
		Renderer.clear();

		//imgui new frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		
		glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);
		glm::mat4 mvp = proj * view * model;

		Shader.setUniformMat4f("u_MVP", mvp);

		//rebound at runtime
		//glUseProgram(shader);
		//glUniform4f(location, r, 0.8, 0.1, 1.0);
		
		//std::cout << glGetUniformLocation(shader, "u_Texture") << std::endl;
		
		//we can remove the arrtibpointer and attribarray from runtime and the bind buffer, just with binding the vao to get the same outcome
		//we are linking the buffer to the vertex array in the vertexarribpointer.
		//va.bind();
		/* this is how it would be used if we only use one vertext array object could potenially be faster so use this way in other programs
		   do not clear the vertex array object if using this way. 
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
		glEnableVertexAttribArray(0);
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		*/
		{
			ImGui::SliderFloat3("Translation", &translation.x, 0.0f, 960.0f);
		}

		//count = number of indices
		//Shader.bind();
		//call(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, indicies);)
		Renderer.draw(va, ib, Shader);
		
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

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		/* Swap front and back buffers */
		glfwSwapBuffers(Renderer.getWindow());

		/* Poll for and process events */
		glfwPollEvents();

		
	}

	//glDeleteProgram(shader);
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	return 0;
}