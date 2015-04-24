// Author  : Lewis Ward
// Program : Animation System Tech Demo
// Date    : 18/04/2015
#pragma once
#include "Mesh.h"

//----------------------------------------------------------------------------------------------------------------------
/// \brief	Mesh that is loaded from 'meshes' file
/// This class should be used for 3D model that DO NOT contain any animation
/// This class has been based but greatly modified from my PGAG Assignment 1 project
//----------------------------------------------------------------------------------------------------------------------
class Object
{
public:
	/// \brief Constructor
	/// \prama const char* mesh file name
	/// \prama const char* mesh collision file name
	Object(const char* meshFile, const char* collisionFile);

	/// \brief Destructor
	~Object();

	/// \brief draw the mesh
	void draw();

	/// \brief set the model martix
	/// \prama glm::mat4 matrix
	inline void matrix(glm::mat4 matrix)
	{ m_matrix = matrix; }

	/// \brief scale the model
	/// \prama glm::mat4 matrix
	void scale(glm::mat4 matrix);

	/// \brief translate the model
	/// \prama glm::vec3 vector
	void translate(glm::vec3 vector);

	/// \brief scale the model
	/// \prama float x
	/// \prama float y
	void rotate(float x, float y);

	/// \brief get the models' translation
	/// \return glm::vec3 vector
	glm::vec3 getTranslation();

	/// \brief get the model matrix
	/// \return glm::mat4 matrix
	inline glm::mat4 matrix()
	{ return m_matrix; }

	/// \brief get the model AABB
	/// \return AABB3 AABB
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
