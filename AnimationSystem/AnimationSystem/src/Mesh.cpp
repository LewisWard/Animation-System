// Author  : Lewis Ward
// Program : Animation System Tech Demo
// Date    : 20/02/2015
#include "Mesh.h"

/* Animated Mesh (.amesh) file format:
// ------------------------------------------ About the format ------------------------------------------ //
   The file contains joint/frame data and mesh data, the joint/frame data is within the first half of
   the file and mesh data in the second half. The first joint should always be the Trajectory followed by 
	 the Hips (provided named in Maya) without the Trajectory or Hips joint the plugin will crash Maya!
	 Any other joint after the first 2 joints can be called anything you like.

	 NOTE: make sure the nmber of vertices is divisible by 3, otherwise the plugin will throw an expection
	 and crash Maya as it cannot output a set (triple) of indices!

// ------------------------------------------  File structure  ------------------------------------------ //
   framesNum											///< uint32_t
   jointsNum											///< uint32_t
	 verticesNum										///< uint32_t
	 indicesNum											///< uint32_t
	 jointClusterNum								///< uint32_t
	 joint index										///< uint32_t
	 joint name											///< string
	 frame													///< int
   positions[f].x									///< vec3
   positions[f].y									///< vec3
   positions[f].z									///< vec3
   rotations[f].x									///< quat (vec4)
   rotations[f].y									///< quat (vec4)
   rotations[f].z									///< quat (vec4)
   rotations[f].w									///< quat (vec4)
   jointCluster	name							///< string
   jointCluster connections				///< uint32_t
   positions[f].x	conection				///< vec3
   positions[f].y	conection				///< vec3
   positions[f].z	conection				///< vec3
	 vertexPositions[f].x						///< vec3
   vertexPositions[f].y 					///< vec3
   vertexPositions[f].z 					///< vec3
   vertexNormal[f].x							///< vec3
   vertexNormal[f].y 							///< vec3
   vertexNormal[f].z 							///< vec3
   vertexUV[f].x 									///< vec2
   vertexUV[f].y 									///< vec2
   indicesA												///< uint32_t
	 indicesB												///< uint32_t
	 indicesC												///< uint32_t
*/

Mesh::Mesh(const char* rig)
{
	m_rMesh.originalMesh = nullptr;

	m_dirX = glm::vec3(1.0f, 0.0f, 0.0f);
	m_dirY = glm::vec3(0.0f, 1.0f, 0.0f);
	m_direction = glm::vec3(0.0f, 0.0f, -1.0f);
	m_hAngle = 0.0f;

	std::cout << "LOADING ANIMATION MESH FILE: ";

	// open file
	std::ifstream ifs(rig);

	char jointClusterName[100];
	uint32_t frames = 0, joints = 0, frameNumber = 0, vertexCount = 0, indexCount = 0, jointClusterCount = 0;
	std::vector<glm::vec3> positions;
	std::vector<glm::vec4> rotations;

	glm::vec3 position;
	glm::vec4 rotation; 

	vertNormalUV meshData;

	std::vector<vertNormalUV> vertices;
	std::vector<int> indices;

	bool read = false;

	// make sure file opened
	while (ifs.is_open())
	{
		// read in the data
		ifs >> frames;
		ifs >> joints;
		ifs >> vertexCount;
		ifs >> indexCount;
		ifs >> m_jointClusterCount;
				
		// read in the joint mapping
		m_jointMapping.resize(joints);
		for (size_t map = 0; map < joints; ++map)
		{
			ifs >> m_jointMapping[map].index;
			ifs.get(jointClusterName, 100);
			m_jointMapping[map].name = jointClusterName;
		}
		
		// resize vectors
		positions.resize(joints * frames);
		rotations.resize(joints * frames);
		m_rMesh.clusters.resize(joints);
		m_indices.resize(positions.size());
		vertices.resize(vertexCount);
		indices.resize(indexCount);

		for (size_t i = 0; i < m_indices.size(); ++i)
			m_indices[i] = i;

		// read vertex and rotation data
		for (size_t frame = 0; frame < (joints * frames); ++frame)
		{
			ifs >> frameNumber
				>> position.x >> position.y >> position.z
				>> rotation.x >> rotation.y >> rotation.z >> rotation.w;

			// store the values
			positions[frame].x = position.x;
			positions[frame].y = position.y;
			positions[frame].z = position.z;
			rotations[frame].x = rotation.x;
			rotations[frame].y = rotation.y;
			rotations[frame].z = rotation.z;
			rotations[frame].w = rotation.w;
		}

		// read in all joint clusters
		for (int i = 0; i < m_jointClusterCount; ++i)
		{
			// read in the joint cluster data
			ifs >> jointClusterName;
			ifs >> jointClusterCount;

			// create and fill joint cluster
			jointCluster tempCluster;
			std::string jointName(jointClusterName);

			// check that the names match, if so we know the index
			int jointIndex = 0;
			for (size_t map = 0; map < m_jointMapping.size(); ++map)
			{
				// get the char array and remove white space from the start
				std::string jointNameOrg(m_jointMapping[map].name);
				jointNameOrg.erase(0, 1);

				// do the check
				if (jointName == jointNameOrg)
				{
					jointIndex = m_jointMapping[map].index;
					break;
				}
			}

			// assign the index and resize the vertives
			tempCluster.joint = jointIndex;
			tempCluster.connectedVerts.resize(jointClusterCount);

			for (int i = 0; i < jointClusterCount; ++i)
			{
				//ifs >> tempCluster.verts[i];
				ifs >> tempCluster.connectedVerts[i].x >> tempCluster.connectedVerts[i].y >> tempCluster.connectedVerts[i].z;
			}

			// add joint cluster
			m_jointCluster.push_back(tempCluster);
		}

		for (size_t vertex = 0; vertex < vertexCount; ++vertex)
		{
			ifs >> meshData.V.x >> meshData.V.y >> meshData.V.z
					>> meshData.N.x >> meshData.N.y >> meshData.N.z
					>> meshData.U.x >> meshData.U.y;

			// store the values
			vertices[vertex] = meshData;
		}


		int indexA;

		// store the indices
		for (size_t index = 0; index < indexCount; index++)
		{
			ifs >> indexA;	
			indices[index] = indexA;
		}

		// compares connected vertex vs all the vertices within the mesh, in order to get all indices for 
		// the same vertex
		for (int j = 0; j < m_jointCluster.size(); ++j)
		{
			for (int i = 0; i < m_jointCluster[j].connectedVerts.size(); ++i)
			{
				for (int ii = 0; ii < vertices.size(); ++ii)
				{
					if (m_jointCluster[j].connectedVerts[i] == vertices[ii].V)
					{
						m_jointCluster[j].verts.push_back(ii);
					}
				}
			}
		}

		// close the file
		ifs.close();

		read = true;
	}


	// if the file was read, we have the data.
	// Now sort the data to have the correct transforms with the correct frames
	if (read)
	{
		// store number of frames and joints
		m_frames = frames;
		m_joints = joints;

		// store vertex and indices count
		m_vertices = vertexCount;
		m_meshIndices = indexCount;

		// set the model martix to the Trajectory joint
		m_modelMatrix[3].x = positions[0].x;
		m_modelMatrix[3].y = positions[0].y;
		m_modelMatrix[3].z = positions[0].z;

		// store the correct joint data into the correct joint cluster
		m_rMesh.clusters.resize(joints);
		for (size_t j = 0; j < joints; ++j)
		{
			int index = j * joints + 1;

			// set the joint index
			m_rMesh.clusters[j].joint = j;

			// set the bind position
			glm::mat4x3 bind;
			bind[3][0] = positions[index].x;
			bind[3][1] = positions[index].y;
			bind[3][2] = positions[index].z;

			m_rMesh.clusters[j].bindPose = bind;

			m_rMesh.deformed.resize(joints * frames);
			m_rMesh.deformedRotations.resize(joints * frames);

			int i = 0;
			int k = 0;
			// store the data into each joint and all of its frame.
			// (i.e joint 1 for all frames, joint 2 for all frames etc...)
			for (size_t f = 0; f < joints * frames; ++f)
			{
				// every time the number of joints is done reset 
				if (f % frames == 0)
				{
					i = 0;
					k++;
				}

				// start at the Hips joint 
				int ii = (i * joints + k);

				if (ii > positions.size() - 1)
					ii = positions.size() - 1;

				m_rMesh.deformed[f].V.x = positions[ii].x;
				m_rMesh.deformed[f].V.y = positions[ii].y;
				m_rMesh.deformed[f].V.z = positions[ii].z;
				m_rMesh.deformedRotations[f].x = rotations[ii].x;
				m_rMesh.deformedRotations[f].y = rotations[ii].y;
				m_rMesh.deformedRotations[f].z = rotations[ii].z;
				m_rMesh.deformedRotations[f].w = rotations[ii].w;
				i++;
			}
		}

		// create original mesh
		m_rMesh.originalMesh = new mesh();
		m_rMesh.originalMesh->verts.resize(m_vertices);
		m_rMesh.originalMesh->quats.resize(rotations.size());
		m_rMesh.meshData.resize(m_vertices);

		// load original vertices
		for (size_t i = 0; i < m_vertices; ++i)
		{
			m_rMesh.meshData[i] = vertices[i];
			m_rMesh.originalMesh->verts[i] = vertices[i];
		}

		// load original quats
		for (size_t i = 0; i < rotations.size(); ++i)
		{
			m_rMesh.originalMesh->quats[i].x = rotations[i].x;
			m_rMesh.originalMesh->quats[i].y = rotations[i].y;
			m_rMesh.originalMesh->quats[i].z = rotations[i].z;
			m_rMesh.originalMesh->quats[i].w = rotations[i].w;
		}

		m_meshDataVertices.resize(m_meshIndices);
		for (size_t i = 0; i < m_meshIndices; ++i)
		{
			m_meshDataVertices[i] = indices[i];
		}

		// genereate a buffer joint data
		glGenBuffers(1, &m_vbo);
		glGenBuffers(1, &m_ibo);
		// bind
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertNormal)*m_rMesh.deformed.size(), &m_rMesh.deformed[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*m_indices.size(), &m_indices[0], GL_STATIC_DRAW);
		// unbind
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		// genereate a buffer for the mesh data
		glGenBuffers(1, &m_vboMesh);
		glGenBuffers(1, &m_iboMesh);
		// bind
		glBindBuffer(GL_ARRAY_BUFFER, m_vboMesh);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertNormalUV)*m_rMesh.meshData.size(), &m_rMesh.meshData[0], GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboMesh);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*m_meshDataVertices.size(), &m_meshDataVertices[0], GL_STATIC_DRAW);
		// unbind
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		std::cout << "LOADED\n";
	}
	else
	{
		std::cout << "FAILED\n";
	}
}
Mesh::~Mesh()
{
	delete m_rMesh.originalMesh;
	m_rMesh.originalMesh = nullptr;
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ibo);
	glDeleteBuffers(1, &m_vboMesh);
	glDeleteBuffers(1, &m_iboMesh);
}
void Mesh::update(float dt, float frame, Event& events, XboxController& controller)
{
	// get lastest event 
	int eventCode = events.update();
	glm::vec2 mouse = events.mouseUpdate();
	glm::vec2 lStick = controller.getLeftStick();
	glm::vec2 rStick = controller.getRightStick();

	glm::vec3 position;
	position.x = m_modelMatrix[3].x;
	position.y = m_modelMatrix[3].y;
	position.z = m_modelMatrix[3].z;

	m_direction = glm::normalize(m_direction);
	glm::vec3 newDirection(m_direction);
	glm::vec3 translation;
	newDirection = newDirection * m_facing;
	translation = newDirection;

	// invert the X 
	translation.x = -translation.x;
	translation *= rStick.y * 2.5f * dt;
	std::cout << translation.x << " " << translation.y << " " << translation.z << std::endl;

	// move
	position += translation;
	m_modelMatrix = glm::translate(position);

	// apply rotation
	m_hAngle += -rStick.x * 5.0f * dt;
	m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(m_hAngle), m_dirY);
	m_facing = glm::quat_cast(m_modelMatrix);


	// cycle all joint clusters
	for (int j = 0; j < m_jointCluster.size(); ++j)
	{
		// data stored in array always starts at the Hips, so minus one to make index match order
		int jointClusterIndex = m_jointCluster[j].joint - 1;

		// cycle all the connections within the cluster
		for (int i = 0; i < m_jointCluster[j].verts.size(); ++i)
		{
			// get indices
			int tripleA = m_jointCluster[j].verts[i];

			// get the first and last frame
			glm::vec3 animFrame(m_rMesh.deformed[j * 24].V);
			glm::vec3 animFrame1(m_rMesh.deformed[j * 24 + 23].V);

			// compute te difference between the frames
			glm::vec3 frameDiff(animFrame1 - animFrame);

			// for all three vertices within the triangle connected to the joint get it start and end position
			glm::vec3 jointAA(m_rMesh.originalMesh->verts[tripleA].V);
			glm::vec3 jointAB(frameDiff + jointAA);

			// linear interpolate all three vertices
			glm::vec3 lerpedA(glm::lerp(jointAA, jointAB, frame / 24.0f));

			// get the rotation for this frame and rotate the lerped vertices
			glm::quat rotation(m_rMesh.deformedRotations[jointClusterIndex * 24 + frame]);

			glm::vec3 lerpedRot = glm::rotate(rotation, lerpedA);

			// update current position
			m_rMesh.meshData[tripleA].V = lerpedA;
		}
	}
	
	// update the mesh data within the VBO, this could be better done for perfomance!
	glBindBuffer(GL_ARRAY_BUFFER, m_vboMesh);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertNormalUV)*m_rMesh.meshData.size(), &m_rMesh.meshData[0]);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void Mesh::draw()
{
	// enable vertex data
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);

	//Const foat pointer that points to a offset of NULL
	const float* coordinate = 0;
	// enable vertex array
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(vertNormal), coordinate);
	glEnableVertexAttribArray(0);

	// enable normal
	glVertexAttribPointer(1, 3, GL_FLOAT, true, sizeof(vertNormal), coordinate + 4);
	glEnableVertexAttribArray(1);

	// draw the indexed triangles from the VBO
	glDrawElements(GL_POINTS, m_indices.size(), GL_UNSIGNED_INT, 0);

	// disable editing of array
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	// unbind buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void Mesh::draw(int frame)
{
	// enable vertex data
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);

	//Const foat pointer that points to a offset of NULL
	const float* coordinate = 0;
	// enable vertex array
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(vertNormal), coordinate);
	glEnableVertexAttribArray(0);

	// enable normal
	glVertexAttribPointer(1, 3, GL_FLOAT, true, sizeof(vertNormal), coordinate + 4);
	glEnableVertexAttribArray(1);

	// draw, based upon: http://stackoverflow.com/questions/9431923/using-an-offset-with-vbos-in-opengl
	glDrawElements(GL_POINTS, 1, GL_UNSIGNED_INT, (void*)(frame * sizeof(GLuint)));

	// disable editing of array
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	// unbind buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void Mesh::fetchDraw(int offset)
{
	// enable vertex data
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);

	//Const foat pointer that points to a offset of NULL
	const float* coordinate = 0;
	// enable vertex array
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(vertNormal), coordinate);
	glEnableVertexAttribArray(0);

	// enable normal
	glVertexAttribPointer(1, 3, GL_FLOAT, true, sizeof(vertNormal), coordinate + 4);
	glEnableVertexAttribArray(1);

	// draw, based upon: http://stackoverflow.com/questions/9431923/using-an-offset-with-vbos-in-opengl
	glDrawElementsBaseVertex(GL_POINTS, m_indices.size(), GL_UNSIGNED_INT, 0, offset);

	// disable editing of array
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	// unbind buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void Mesh::drawObject()
{
	// enable vertex data
	glBindBuffer(GL_ARRAY_BUFFER, m_vboMesh);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboMesh);

	//Const foat pointer that points to a offset of NULL
	const float* coordinate = 0;
	// enable vertex array
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(vertNormalUV), coordinate);
	glEnableVertexAttribArray(0);

	// enable normal
	glVertexAttribPointer(1, 3, GL_FLOAT, true, sizeof(vertNormalUV), coordinate + 3);
	glEnableVertexAttribArray(1);
	
	// enable UV
	glVertexAttribPointer(2, 2, GL_FLOAT, true, sizeof(vertNormalUV), coordinate + 6);
	glEnableVertexAttribArray(2);

	// draw
	glDrawElements(GL_TRIANGLES, m_meshDataVertices.size(), GL_UNSIGNED_INT, 0);

	// disable editing of array
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	// unbind buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void Mesh::drawObject(int frame)
{
	// enable vertex data
	glBindBuffer(GL_ARRAY_BUFFER, m_vboMesh);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboMesh);

	//Const foat pointer that points to a offset of NULL
	const float* coordinate = 0;
	// enable vertex array
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(vertNormalUV), coordinate);
	glEnableVertexAttribArray(0);

	// enable normal
	glVertexAttribPointer(1, 3, GL_FLOAT, true, sizeof(vertNormalUV), coordinate + 4);
	glEnableVertexAttribArray(1);

	// enable UV
	glVertexAttribPointer(2, 2, GL_FLOAT, true, sizeof(vertNormalUV), coordinate + 8);
	glEnableVertexAttribArray(2);

	// draw
	glDrawElements(GL_TRIANGLES, 1, GL_UNSIGNED_INT, (void*)(frame * sizeof(GLuint)));

	// disable editing of array
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	// unbind buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
