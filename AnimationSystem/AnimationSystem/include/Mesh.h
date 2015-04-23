// Author  : Lewis Ward
// Program : Animation System Tech Demo
// Date    : 20/02/2015
#pragma once
#include "Texture.h"
#include "Events.h"
#include "Xbox.h"
#include <vector>
#include <fstream>   
#define NUM_OF_FRAMES 72.0

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

struct AABB2
{
	glm::vec2 min;
	glm::vec2 max;
	float width;
	float height;
	glm::vec2 center;

	AABB2() : min(0), max(1) {}; ///< DO NOT USE UNLESS YOU WILL COMPUTE WIDTH/HEIGHT/CENTER!
	AABB2(glm::vec2 minium, glm::vec2 maxium) : min(minium), max(maxium),
																							width(maxium.x - minium.x), height(maxium.y - minium.y),
																							center((maxium.x + minium.x) / 2, (maxium.y + minium.y) / 2) {};
	bool contains(glm::vec2 one)
	{
		if (one.x >= min.x && one.x <= max.x)
		{
			if (one.y >= min.y && one.y <= max.y)
			{
				return true;
			}
		}

		return false;
	}

	// based upon : http://gamedev.stackexchange.com/questions/60505/how-to-check-for-cube-collisions
	bool intersect(AABB2 one)
	{
		if (glm::abs(one.center.x - center.x) < one.width / 2 + width / 2)
		{
			if (glm::abs(one.center.y - center.y) < one.height / 2 + height / 2)
			{
				return true;
			}
		}

		return false;
	}
};

struct AABB3
{
	glm::vec3 min;
	glm::vec3 max;
	float width;
	float height;
	glm::vec3 center;

	AABB3() : min(0), max(1) {}; ///< DO NOT USE UNLESS YOU WILL COMPUTE WIDTH/HEIGHT/CENTER!
	AABB3(glm::vec3 minium, glm::vec3 maxium) : min(minium), max(maxium),
																							width(maxium.x - minium.x), height(maxium.y - minium.y),
																							center((maxium.x + minium.x) / 2, (maxium.y + minium.y) / 2, (maxium.z + minium.z) / 2) {};

	bool contains(glm::vec3 one)
	{
		if (one.z >= min.z && one.z <= max.z)
		{
			if (one.x >= min.x && one.x <= max.x)
			{
				if (one.y >= min.y && one.y <= max.y)
				{
					return true;
				}
			}
		}

		return false;
	}

	bool intersect(AABB3 one)
	{
		if (glm::abs(one.center.z - center.z) < one.width / 2 + width / 2)
		{
			if (glm::abs(one.center.x - center.x) < one.width / 2 + width / 2)
			{
				if (glm::abs(one.center.y - center.y) < one.height / 2 + height / 2)
				{
					return true;
				}
			}
		}

		return false;
	}
};

//----------------------------------------------------------------------------------------------------------------------
/// \brief	Mesh that is loaded from 'amesh' (Animation Mesh) file
//----------------------------------------------------------------------------------------------------------------------
class Mesh
{
public:
	/// \brief	ctor
	/// \param const char* filename of the file that contains the rig (animation data)
	///\prama const char* mesh collision file name
	Mesh(const char* rig, const char* collisionFile);
	/// \brief  dtor
	~Mesh();

	void update(float dt, float frame, Event& events, bool movement[], XboxController& controller);

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

	inline float getHAngle()
	{ return m_hAngle; }

	inline void hAngle(float h)
	{ m_hAngle = h; }

	inline AABB3 getAABB()
	{ return m_AABB; }

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
	AABB3 m_AABB;
};
