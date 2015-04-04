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
		m_indices.resize(positions.size());

		for (size_t i = 0; i < m_indices.size(); ++i)
			m_indices[i] = i;

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

	// genereate a buffer
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

}
Mesh::~Mesh()
{
	m_rMesh.originalMesh = nullptr;
	glDeleteBuffers(1, &m_vbo);
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
	glDrawElements(GL_POINTS, m_indices.size(), GL_UNSIGNED_INT, (void*)(frame * sizeof(GLuint)));

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
