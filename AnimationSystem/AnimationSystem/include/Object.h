// Author  : Lewis Ward
// Program : Animation System Tech Demo
// Date    : 18/04/2015
#pragma once
#include "Mesh.h"

//----------------------------------------------------------------------------------------------------------------------
/// \brief	Mesh that is loaded from 'meshes' file
/// This class should be used for 3D model that DO NOT contain any animation
/// This class has been taken but modified from my PGAG Assignment 1 project
//----------------------------------------------------------------------------------------------------------------------
class Object
{
public:
	///\brief Constructor
	///\prama const char* mesh file name
	Object(const char* meshFile);
	///\brief Destructor
	~Object();

	///\brief draw the mesh
	void draw();

	inline void matrix(glm::mat4 matrix)
	{ m_matrix = matrix; }

	inline void scale(glm::mat4 matrix)
	{ m_matrix *= matrix; }

	inline glm::mat4 matrix()
	{ return m_matrix; }

private:
	std::vector<vertNormalUV> m_vertexData; ///< vertex data (vertex, normal, uv)
	std::vector<int> m_indices; ///< indices
	uint32_t m_vbo; ///< VBO
	uint32_t m_ibo; ///< IBO
	glm::mat4 m_matrix; ///< model matrix
};
