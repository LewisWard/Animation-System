// Author  : Lewis Ward
// Program : Animation System Tech Demo
// Date    : 20/02/2015
#include "Mesh.h"

// animation file formate
// framesNum			///< uint32_t
// jointsNum			///< uint32_t
// positions[f].x	///< vec3
// positions[f].y	///< vec3
// positions[f].z	///< vec3
// rotations[f].x	///< quat (vec4)
// rotations[f].y	///< quat (vec4)
// rotations[f].z	///< quat (vec4)
// rotations[f].w	///< quat (vec4)
// the first joint should always be the Trajectory followed by the Hips (provided named in Maya)

Mesh::Mesh(const char* rig)
{
	m_rMesh.originalMesh = nullptr;

	// open file
	std::ifstream ifs(rig);

	uint32_t frames = 0, joints = 0, frameNumber = 0;
	std::vector<glm::vec3> positions;
	std::vector<glm::vec4> rotations;

	glm::vec3 position;
	glm::vec4 rotation; 

	bool read = false;

	// make sure file opened
	while (ifs.is_open())
	{
		// read in the data
		ifs >> frames;
		ifs >> joints;

		// resize vectors
		positions.resize(joints * frames);
		rotations.resize(joints * frames);
		m_rMesh.clusters.resize(joints);

		std::cout << frames << std::endl << joints << std::endl;


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

			std::cout << frameNumber << " " << positions[frame].x << " " << positions[frame].y << " " << positions[frame].z <<
		  " " << rotations[frame].x << " " << rotations[frame].y << " " << rotations[frame].z << " " << rotations[frame].w << std::endl;
		}

		// close the file
		ifs.close();

		read = true;
	}


	// if the file was read, we have the data.
	// Now sort the data to have the correct transforms with the correct frames
	if (read)
	{
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
				std::cout << i << " " << k << std::endl;
				i++;
			}
		}
	}

}
Mesh::~Mesh()
{
	m_rMesh.originalMesh = nullptr;
}
