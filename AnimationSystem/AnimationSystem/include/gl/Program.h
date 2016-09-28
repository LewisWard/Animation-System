// Author : Lewis Ward (http://www.lewis-ward.com)
// Program : Animation System Tech Demo
// Date    : 09/02/2015
// © 2015-2016 Lewis Ward. All rights reserved.
#pragma once
#include "Shader.h"

// This namespace/class has been modified from my PGAG Assignment 1 project
// GL Shaders namespace
namespace gls
{
//----------------------------------------------------------------------------------------------------------------------
/// \brief  Program for shader
//----------------------------------------------------------------------------------------------------------------------
class Program
{
public:
	/// \brief Constructor
	Program() : m_program(0){};
	/// \brief Destructor
	~Program();

	/// \brief creates a shader program
	/// \prama Shader* vertex shader
	/// \prama Shader* fragment shader
	bool create(const Shader* const vertex, const Shader* const fragment);

	/// \brief bind the program
	void bind() const;

	/// \brief unbind the program
	void unbind() const;

	/// \brief get the shader program
	/// \prama uint32_t* program
	inline uint32_t program()
	{ return m_program; }

	/// \brief set the uniform
	/// \prama const char* name of the uniform
	/// \prama float value
	void uniform_1i(const char* uniformName, float value);

	/// \brief set the uniform
	/// \prama float value
	void uniform_1f(const char* uniformName, float value);

	/// \brief set the uniform
	/// \prama const char* name of the uniform
	/// \prama float X
	/// \prama float Y
	/// \prama float Z
	/// \prama float W
	void uniform_4f(const char* uniformName, float floatZero, float floatOne, float floatTwo, float floatThree);
	
	/// \brief set the uniform
	/// \prama const char* name of the uniform
	/// \pramasize_t count
	/// \pramabool transpose
	/// \pramafloat[] 16 floats (from matrix)
	void uniform_Matrix4(const char* uniformName, size_t count, bool transpose, float mvpFloat[]);

	///// ------------------------------------- GLM LIBRARY VERSIONS ------------------------------------- /////

	/// \brief set the uniform
	/// \prama char* name of the uniform
	/// \prama glm::vec3 vector 
	void uniform_3f(const char* uniformName, glm::vec3 vector);

	/// \brief set the uniform
	/// \prama const char* name of the uniform
	/// \prama glm::vec4 vector 
	void uniform_4f(const char* uniformName, glm::vec4 vector);

	/// \brief set the uniform
	/// \prama char* name of the uniform
	/// \prama size_t count
	/// \prama bool transpose
	/// \prama glm::mat4x4 matrix
	void uniform_Matrix4(const char* uniformName, size_t count, bool transpose, glm::mat4x4 matrix);

private:
	uint32_t m_program; ///< shader program
};
////////////////////////////
}; // end of gls namespace
////////////////////////////
