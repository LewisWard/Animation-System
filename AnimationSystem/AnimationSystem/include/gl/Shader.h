// Author  : Lewis Ward
// Program : Animation System Tech Demo
// Date    : 09/02/2015
#pragma once
#include "Window.h"
#include <fstream>
#include <string>

// This namespace/class has been directly taken from my PGAG Assignment 1 project
namespace gls
{
// type of shader (vertex or fragment)
enum shaderType
{
	sVERTEX, sFRAGMENT
};

/// \brief get a gl error within shader
/// \prama const char* shader file
void getGLError(const char* file);

//----------------------------------------------------------------------------------------------------------------------
/// \brief  GLSL Shader
//----------------------------------------------------------------------------------------------------------------------
class Shader
{
public:
	///\brief Constr, create and compile shader
	///\prama const char* shader source code
	Shader(const char* shaderFile, shaderType type);
	/// \brief Destr
	~Shader();

	///\brief get the shader object
	///\return uint32_t shader object
	inline uint32_t shader() const
	{ return m_shader; }

private:
	shaderType m_type;
	uint32_t m_shader;
	std::string m_filename;
	std::string m_source;

	/// \brief load a shader from file, should be called before creating a shader
	/// \prama const char* shader file
	char* loadShaderFile(const char* shaderFilename);
};
////////////////////////////
}; // end of gls namespace
////////////////////////////
