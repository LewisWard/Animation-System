// Author  : Lewis Ward
// Program : Animation System Tech Demo
// Date    : 09/02/2015
#include "gl/Program.h"

gls::Program::~Program()
{
	glDeleteProgram(m_program);
	getGLError("Program::~Program");
}
bool gls::Program::create(const Shader* const vertex, const Shader* const fragment)
{
	// some of this code is based on code by Dr Leigh McLoughlin and his 'OpenGLTriangleCode' example
	// also some based from : http://www.nexcius.net/2012/11/20/how-to-load-a-glsl-shader-in-opengl-using-c/

	// delete program, make sure that there isn't any previous programs'
	if(m_program)
	{
		glDeleteProgram(m_program);
		getGLError("Program::create");
	}

	// create new program
	m_program = glCreateProgram();
	// check for errors
	getGLError("Program::create");

	// attach the shaders to the program
	glAttachShader(m_program, vertex->shader());
	glAttachShader(m_program, fragment->shader());
	getGLError("Program::create");

	// link the program (connects vertex and fragment shaders together)
	glLinkProgram(m_program);
	getGLError("Program::create");

	// check that linking was good
	int32_t linked = GL_FALSE;
	glGetProgramiv(m_program, GL_LINK_STATUS, &linked);
	getGLError("Program::create");

	// will print the error of were the linking failed
	if(linked == GL_FALSE)
	{
		getGLError("Program::create");
		// what went wrong?
		GLsizei length = 0;
		char linkBuffer[1024]; 
    glGetProgramInfoLog(m_program, 1024, &length, linkBuffer);
		getGLError("Program::create");
		// print to console
		std::cout<<"Program linking failed: "<<linkBuffer<<std::endl;
		// delete program
		glDeleteProgram(m_program);
		getGLError("Program::create");
		m_program = 0;
		// return that linking failed
		return false;
	}
	return true;
}
void gls::Program::bind() const
{
	// bind the program for use
	glUseProgram(m_program);
}
void gls::Program::unbind() const
{
	// stop using the program
	glUseProgram(0);
}
void gls::Program::uniform_1i(const char* uniformName, float value)
{
	glUniform1i(glGetUniformLocation(program(), uniformName), value);
}
void gls::Program::uniform_1f(const char* uniformName, float value)
{
	glUniform1f(glGetUniformLocation(program(), uniformName), value);
}
void gls::Program::uniform_4f(const char* uniformName, float floatZero, float floatOne, float floatTwo, float floatThree)
{
	glUniform4f(glGetUniformLocation(program(), uniformName), floatZero, floatOne, floatTwo, floatThree);
}
void gls::Program::uniform_Matrix4(const char* uniformName, size_t count, bool transpose, float mvpFloat[])
{
	glUniformMatrix4fv(glGetUniformLocation(program(), uniformName), count, transpose, mvpFloat);
}

///// ------------------------------------- GLM LIBRARY VERSIONS ------------------------------------- /////
void gls::Program::uniform_Matrix4(const char* uniformName, size_t count, bool transpose, glm::mat4x4 matrix)
{
	glUniformMatrix4fv(glGetUniformLocation(program(), uniformName), count, transpose, glm::value_ptr(matrix));
}
void gls::Program::uniform_3f(const char* uniformName, glm::vec3 vector)
{
	glUniform4f(glGetUniformLocation(program(), uniformName), vector.x, vector.y, vector.z, 0.0f);
}
void gls::Program::uniform_4f(const char* uniformName, glm::vec4 vector)
{
	glUniform4f(glGetUniformLocation(program(), uniformName), vector.x, vector.y, vector.z, vector.w);
}
