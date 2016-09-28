// Author : Lewis Ward (http://www.lewis-ward.com)
// Program : Animation System Tech Demo
// Date    : 20/02/2015
// © 2015-2016 Lewis Ward. All rights reserved.
#pragma once
#include "Texture.h"
#include "Events.h"
#include "Xbox.h"
#include <vector>
#include <fstream>   
#define NUM_OF_FRAMES 72.0

/// \brief vertex data
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
	std::vector<glm::quat> quats; ///< all the quats within the mesh
	std::vector<vertNormalUV> verts; ///< all the vertices/normals/uvs
	std::vector<int> indices; ///< indices
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
	int index; ///< joint index
	std::string name; ///< joint name
};

struct rigidSkinnedMesh
{
	mesh* originalMesh; ///< original mesh (never deformed)
	std::vector<jointCluster> clusters; ///< all the joint clusters 
	std::vector<vertNormal> deformed; ///< which vertices etc. get deformed
	std::vector<glm::quat> deformedRotations; ///< which quats get deformed
	std::vector<vertNormalUV> meshData; ///< overall mesh data
};

/// \brief Axis-Aligned-Bounding-Box (2D)
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
	/// \brief tests if a point is within the AABB
	/// \prama glm::vec2 point
	/// \bool true if contains point
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
	/// \brief tests if a AABB intersects the AABB
	/// \prama AABB2 aabb
	/// \bool true if contains point
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

/// \brief Axis-Aligned-Bounding-Box (3D)
struct AABB3
{
	glm::vec3 min;
	glm::vec3 max;
	float width;
	float height;
	float depth;
	glm::vec3 center;

	AABB3() : min(0), max(1) {}; ///< DO NOT USE UNLESS YOU WILL COMPUTE WIDTH/HEIGHT/CENTER!
	AABB3(glm::vec3 minium, glm::vec3 maxium) : min(minium), max(maxium),
																							width(maxium.x - minium.x), height(maxium.y - minium.y), depth(maxium.z - minium.z),
																							center((maxium.x + minium.x) / 2, (maxium.y + minium.y) / 2, (maxium.z + minium.z) / 2) {};

	/// \brief tests if a point is within the AABB
	/// \prama glm::vec3 point
	/// \bool true if contains point
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

	/// \brief tests if a AABB intersects the AABB
	/// \prama AABB3 aabb
	/// \bool true if contains point
	bool intersect(AABB3 one)
	{
		if (glm::abs(one.center.z - center.z) < one.depth / 2 + depth / 2)
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
	/// \prama const char* mesh collision file name
	Mesh(const char* rig, const char* collisionFile);

	/// \brief  dtor
	~Mesh();

	/// \brief updates the animation cycle
	/// \param float delta-time
	/// \prama float current frame
	/// \prama Event event handler
	/// \prama bool[] array of keyboard inputs
	/// \prama XboxController Xbox 360 controller
	void update(float dt, float frame, Event& events, bool movement[], XboxController& controller);

	/// \brief draw the joints
	void draw();

	/// \brief draw frame
	/// \prama int frame
	void draw(int frame);

	/// \brief draw frame using offest
	/// \prama int frame
	void fetchDraw(int offset);

	/// \brief draw the mesh
	void drawObject();

	/// \brief draw the mesh
	/// \prama int frame
	void drawObject(int frame);

	/// \brief get the mesh
	/// \return rigidSkinnedMesh mesh
	inline rigidSkinnedMesh getMesh()
	{ return m_rMesh; }

	/// \brief get the modle matrix
	/// \return glm::mat4 matrix
	inline glm::mat4 getModelMatrix()
	{ return m_modelMatrix; }

	/// \brief set the modle matrix
	/// \prama glm::mat4 matrix
	inline void setModelMatrix(glm::mat4 matrix)
	{ m_modelMatrix = matrix; }

	/// \brief get the total number of frames
	/// \return int frames
	inline int numberOfFrames()
	{ return m_frames; }

	/// \brief get the total number of joints
	/// \return int joints
	inline int numberOfJoints()
	{ return m_joints; }

	/// \brief get horizontal angle
	/// \return float angle
	inline float getHAngle()
	{ return m_hAngle; }

	/// \brief set horizontal angle
	/// \prama float angle
	inline void hAngle(float h)
	{ m_hAngle = h; }

	/// \brief get AABB
	/// \return AABB3 aabb
	inline AABB3 getAABB()
	{ return m_AABB; }

	/// \brief set AABB
	/// \prama AABB3 aabb
	inline void setAABB(AABB3 aabb)
	{ m_AABB = aabb; }

private:
	rigidSkinnedMesh m_rMesh; ///< rigid skinned mesh exported from Maya
	uint32_t m_vbo; ///< VBO
	uint32_t m_ibo; ///< IBO
	uint32_t m_vboMesh; ///< mesh VBO
	uint32_t m_iboMesh; ///< mesh IBO
	std::vector<int> m_indices; ///< indices
	std::vector<int> m_meshDataVertices; ///< mesh vertices
	std::vector<jointCluster> m_jointCluster; ///< mesh joint cluster
	std::vector<jointMap> m_jointMapping; ///< maps all the joints to a index within the mesh
	glm::mat4 m_modelMatrix; ///< model matrix
	int m_frames; ///< number of frames
	int m_joints; ///< number of joints
	int m_vertices; ///< number of vertices
	int m_meshIndices; ///< number of indices
	int m_jointClusterCount; ///< number of joint clusters
	glm::vec3 m_dirX; ///< 'up' direction of the mesh
	glm::vec3 m_dirY; ///< 'up' direction of the mesh
	float m_vAngle; ///< vertical angle of the mesh
	float m_hAngle; ///< horizontal angle of the mesh
	glm::vec3 m_direction; ///< moving direction of the mesh
	glm::quat m_facing; ///< the direction the mesh is facing
	AABB3 m_AABB; ///< AABB for collision
	AABB3 m_AABBOrignal; ///< AABB for collision (never changed)
};
