#include "shader.h"

unsigned int shader::compileShader(const std::string& source, unsigned int type)
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

unsigned int shader::createShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = compileShader(vertexShader, GL_VERTEX_SHADER);
	unsigned int fs = compileShader(fragmentShader, GL_FRAGMENT_SHADER);

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

shader::shader(const std::string& filepath)
{
	FILE* fp;

	fp = fopen(filepath.c_str(), "r+");

	if (!fp)
	{
		std::cout << "file cannot be read" << std::endl;
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

		if (line[8] == *"v" && line[9] == *"e" && line[10] == *"r")
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

	vertexShader = ss[(int)shaderType::VERTEX].str();
	fragmentShader = ss[(int)shaderType::FRAGMENT].str();

	m_RendererID = createShader(vertexShader, fragmentShader);
	glUseProgram(m_RendererID);


}

shader::~shader()
{
	glDeleteShader(m_RendererID);
}

void shader::bind() const
{
	glUseProgram(m_RendererID);

}

void shader::unbind() const
{
	glUseProgram(0);

}
