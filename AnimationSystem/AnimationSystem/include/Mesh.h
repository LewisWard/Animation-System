// Author  : Lewis Ward
// Program : Animation System Tech Demo
// Date    : 20/02/2015
#pragma once
#include "Texture.h"
#include <vector>
#include <fstream>   

///\brief vertex data
struct vertNormal
{
	glm::vec3 V; ///< vertex
	glm::vec3 N; ///< normal
};
struct vertUV
{
	glm::vec3 V; ///< vertex
	glm::vec2 U; ///< UV
};
struct vertNormalUV
{
	glm::vec3 V; ///< vertex
	glm::vec3 N; ///< normal
	glm::vec2 U; ///< UV
};

struct mesh
{
	std::vector<vertNormal> verts;
	std::vector<int> indices;
};

struct jointCluster
{
	int joint; ///< joint index
	glm::mat4x3 bindPose; ///< bind pose in inverse world space
	std::vector<int> verts; ///< indices of the vertices within this joint cluster
};

struct rigidSkinnedMesh
{
	mesh* originalMesh;
	std::vector<jointCluster> clusters;
	std::vector<vertNormal> deformed;
};

//----------------------------------------------------------------------------------------------------------------------
/// \brief	Mesh that is loaded from 'amesh' (Animation Mesh) file
//----------------------------------------------------------------------------------------------------------------------
class Mesh
{
public:

	/// \brief	ctor
	/// \param const char* filename of the file that contains the rig (animation data)
	Mesh(const char* rig);
	/// \brief  dtor
	~Mesh();

	inline rigidSkinnedMesh getMesh()
	{ return m_rMesh; }

private:
	rigidSkinnedMesh m_rMesh;
};
