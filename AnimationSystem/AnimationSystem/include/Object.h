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
	///\prama const char* mesh collision file name
	Object(const char* meshFile, const char* collisionFile);
	///\brief Destructor
	~Object();

	///\brief draw the mesh
	void draw();

	inline void matrix(glm::mat4 matrix)
	{ m_matrix = matrix; }

	void scale(glm::mat4 matrix);

	void translate(glm::vec3 vector);

	void rotate(float x, float y);

	glm::vec3 getTranslation();

	inline glm::mat4 matrix()
	{ return m_matrix; }

	inline AABB3 getAABB()
	{ return m_AABB; }

private:
	std::vector<vertNormalUV> m_vertexData; ///< vertex data (vertex, normal, uv)
	std::vector<int> m_indices; ///< indices
	uint32_t m_vbo; ///< VBO
	uint32_t m_ibo; ///< IBO
	glm::mat4 m_matrix; ///< model matrix
	glm::vec3 m_dirX; ///< for working out the 'up' direction of the object
	glm::vec3 m_dirY; ///< for working out the 'up' direction of the object
	AABB3 m_AABB; ///< for collision detection
};
