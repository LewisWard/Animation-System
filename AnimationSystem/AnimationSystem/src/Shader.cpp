// Author  : Lewis Ward
// Program : Animation System Tech Demo
// Date    : 09/02/2015
#include "gl/Shader.h"

namespace gls
{
void getGLError(const char* file)
{
	// get OpenGL error and print it to console
	switch(glGetError())
	{
	case GL_NO_ERROR: break;
	case GL_INVALID_ENUM: std::cout<< file <<" GL_INVALID_ENUM\n"; break;
	case GL_INVALID_VALUE: std::cout<< file <<" GL_INVALID_VALUE\n"; break;
	case GL_INVALID_OPERATION: std::cout<< file <<" GL_INVALID_OPERATION\n"; break;
	case GL_INVALID_FRAMEBUFFER_OPERATION: std::cout<< file <<" GL_INVALID_FRAMEBUFFER_OPERATION\n"; break;
	case GL_OUT_OF_MEMORY: std::cout<< file <<" GL_OUT_OF_MEMORY\n"; break;
	case GL_STACK_UNDERFLOW: std::cout<< file <<" GL_STACK_UNDERFLOW\n"; break;
	case GL_STACK_OVERFLOW: std::cout<< file <<" GL_STACK_OVERFLOW\n"; break;
	}
}

Shader::Shader(const char* shaderFile, shaderType type)
{
	// some of this code is based on code by Dr Leigh McLoughlin and his 'OpenGLTriangleCode' example
	// also some based from : http://www.nexcius.net/2012/11/20/how-to-load-a-glsl-shader-in-opengl-using-c/

	// open file and create shader source code
	const char* shaderSourceCode = loadShaderFile(shaderFile);

	// make sure we dont have a bad point!
	if(shaderSourceCode)
	{
		// get the name of the file
		m_filename = shaderFile;

		// what type of shader is it?
		switch(type)
		{
		case sVERTEX: m_shader = glCreateShader(GL_VERTEX_SHADER); break;
		case sFRAGMENT: m_shader = glCreateShader(GL_FRAGMENT_SHADER); break;
		}
		// check for an error with the shader
		getGLError(shaderFile);

		// if shader was created
		if(m_shader)
		{
			// set the shader source to the shader object and check for error
			glShaderSource(m_shader, 1, &shaderSourceCode, NULL);
			// compile the shader and check for error
			glCompileShader(m_shader);
			getGLError(shaderFile);

			// we query the compiled shader for errors
			int32_t shaderCompiled = 0;
			glGetShaderiv(m_shader, GL_COMPILE_STATUS, &shaderCompiled);
			getGLError(shaderFile);

			// if something failed then error printed
			GLsizei length = 0;
			char errorBuffer[256] = {0};
			glGetShaderInfoLog(m_shader, 256, &length, errorBuffer);
			// print to console
			std::cout<<errorBuffer<<std::endl;

			// shader source code
			m_source = shaderSourceCode;

			// if failed to compile then delete shader
			if(!shaderCompiled)
			{
				glDeleteShader(m_shader);
				getGLError(shaderFile);
				std::cout<<"\nVertex shader compile failed\n";
				m_shader = 0; // set pointer to NULL
				m_source = "\0"; // source set to 'nothing' 
			}
		}
	}
	else
	{
		std::cout<<"Bad pointer for: shaderSourceCode\n";
	}
}
Shader::~Shader()
{
	// delete shader
	glDeleteShader(m_shader);
}
char* Shader::loadShaderFile(const char* shaderFilename)
{
	// most of this code for load a glsl file is based upon link below: 
	// https://www.opengl.org/sdk/docs/tutorials/ClockworkCoders/loading.php
	// http://www.nexcius.net/2012/11/20/how-to-load-a-glsl-shader-in-opengl-using-c/

	std::string shaderSourceString;
	std::ifstream ifs(shaderFilename, std::ios::in);

	// check the file is open, if not print it failed to open and stop
	if (!ifs.is_open())
	{
		std::cout << "Failed to load shader source: " << shaderFilename << std::endl;
		return 0; // failed
	}

	// go to the end of the file and get the file length, then go back to the beginning of the file
	ifs.seekg(0, std::ios::end);
	size_t len = ifs.tellg();
	ifs.seekg(std::ios::beg);

	std::string endOfLine = "";
	// as long as it is not the end of the file
	while (!ifs.eof()) 
	{
		// get the next line
		std::getline(ifs, endOfLine);
		// add to end of string
		shaderSourceString.append(endOfLine + "\n");
	}

	// create char array
	char* shaderSource = new char[len + 1];
	// convert from string to char array (copy data to array)
	std::strcpy(shaderSource, shaderSourceString.c_str());

	// clsoe the file 
	ifs.close();

	return shaderSource;
}
};
