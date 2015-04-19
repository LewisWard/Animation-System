// Author  : Lewis Ward
// Program : Animation System Tech Demo
// Date    : 20/02/2015
#pragma once
#include "Texture.h"
#include "Events.h"
#include "Xbox.h"
#include <vector>
#include <fstream>   
#define NUM_OF_FRAMES 24.0

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
	std::vector<glm::quat> quats;
	std::vector<vertNormalUV> verts;
	std::vector<int> indices;
};

struct jointCluster
{
	int joint; ///< joint index
	glm::mat4x3 bindPose; ///< bind pose in inverse world space
	std::vector<int> verts; ///< indices of the vertices within this joint cluster
	std::vector<glm::vec3> connectedVerts; ///< the connected vertices
};

struct jointMap
{
	int index;
	std::string name;
};

struct rigidSkinnedMesh
{
	mesh* originalMesh;
	std::vector<jointCluster> clusters;
	std::vector<vertNormal> deformed;
	std::vector<glm::quat> deformedRotations;
	std::vector<vertNormalUV> meshData;
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

	void update(float dt, float frame, Event& events, XboxController& controller);

	void draw();

	void draw(int frame);

	void fetchDraw(int offset);
	
	void drawObject();

	void drawObject(int frame);

	inline rigidSkinnedMesh getMesh()
	{ return m_rMesh; }

	inline glm::mat4 getModelMatrix()
	{ return m_modelMatrix; }

	inline void setModelMatrix(glm::mat4 matrix)
	{ m_modelMatrix = matrix; }

	inline int numberOfFrames()
	{ return m_frames; }

	inline int numberOfJoints()
	{ return m_joints; }

private:
	rigidSkinnedMesh m_rMesh;
	uint32_t m_vbo;
	uint32_t m_ibo;
	uint32_t m_vboMesh;
	uint32_t m_iboMesh;
	std::vector<int> m_indices;
	std::vector<int> m_meshDataVertices;
	std::vector<jointCluster> m_jointCluster;
	std::vector<jointMap> m_jointMapping;
	glm::mat4 m_modelMatrix;
	int m_frames;
	int m_joints;
	int m_vertices;
	int m_meshIndices;
	int m_jointClusterCount;
	glm::vec3 m_dirX; ///< for working out the 'up' direction of the mesh
	glm::vec3 m_dirY; ///< for working out the 'up' direction of the mesh
	float m_vAngle;
	float m_hAngle;
	glm::vec3 m_direction;
	glm::quat m_facing;
};
