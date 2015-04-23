// Author  : Lewis Ward
// Program : Animation System Tech Demo
// Date    : 18/04/2015
#include "Object.h"

Object::Object(const char* meshFile, const char* collisionFile)
{
	std::cout << "LOADING OBJECT MESH FILE: ";

	m_dirX = glm::vec3(1.0f, 0.0f, 0.0f);
	m_dirY = glm::vec3(0.0f, 1.0f, 0.0f);

	// gen buffers
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ibo);

	char meshName[100];
	size_t objectCount, vertexCount, indicesCount;

	// input file stream
	std::ifstream ifs(meshFile);

	// if the file is open
	if (ifs.is_open())
	{
		// read in data
		ifs >> objectCount;
		ifs >> meshName;
		ifs >> vertexCount;
		ifs >> indicesCount;

		// resize vectors
		m_vertexData.resize(vertexCount);
		m_indices.resize(indicesCount);

		float requiredPadding = 0;

		// read vertex data into vector
		for (size_t i = 0; i < m_vertexData.size(); ++i)
		{
			ifs >> m_vertexData[i].V.x >> m_vertexData[i].V.y >> m_vertexData[i].V.z // vertex
				>> m_vertexData[i].N.x >> m_vertexData[i].N.y >> m_vertexData[i].N.z // normal
				>> requiredPadding >> requiredPadding >> requiredPadding // bi-normal
				>> requiredPadding >> requiredPadding >> requiredPadding // tangent
				>> m_vertexData[i].U.x >> m_vertexData[i].U.y; // UV
		}
		// read indices into vector
		for (size_t i = 0; i < m_indices.size(); ++i)
		{
			ifs >> m_indices[i];
		}

		// close the file, we are done with it
		ifs.close();


		// like before read in the data from the file
		std::ifstream ifsCollision(collisionFile);
		std::vector<vertNormalUV> collisionData;

		// if the file is open
		if (ifsCollision.is_open())
		{
			// read in data
			ifsCollision >> objectCount;
			ifsCollision >> meshName;
			ifsCollision >> vertexCount;
			ifsCollision >> indicesCount;

			// will not need indices data as will not be rendering it, just need to construct AABB from vertex data
			collisionData.resize(vertexCount);

			// read vertex data into vector
			for (size_t i = 0; i < collisionData.size(); ++i)
			{
				ifsCollision >> collisionData[i].V.x >> collisionData[i].V.y >> collisionData[i].V.z // vertex
					>> collisionData[i].N.x >> collisionData[i].N.y >> collisionData[i].N.z // normal
					>> requiredPadding >> requiredPadding >> requiredPadding // bi-normal
					>> requiredPadding >> requiredPadding >> requiredPadding // tangent
					>> collisionData[i].U.x >> collisionData[i].U.y; // UV
			}

			// close the file, we are done with it
			ifsCollision.close();

			float smallestX, smallestY, smallestZ, biggestX, biggestY, biggestZ;
			smallestX = collisionData[0].V.x;
			smallestY = collisionData[0].V.y;
			smallestZ = collisionData[0].V.z;
			biggestX = collisionData[0].V.x;
			biggestY = collisionData[0].V.y;
			biggestZ = collisionData[0].V.z;

			// find the the min and max vertices for the AABB
			for (size_t i = 1; i < collisionData.size(); ++i)
			{
				if (collisionData[i].V.x < smallestX)
					smallestX = collisionData[i].V.x;

				if (collisionData[i].V.y < smallestY)
					smallestY = collisionData[i].V.y;

				if (collisionData[i].V.z < smallestZ)
					smallestZ = collisionData[i].V.z;

				if (collisionData[i].V.x > biggestX)
					biggestX = collisionData[i].V.x;

				if (collisionData[i].V.y > biggestY)
					biggestY = collisionData[i].V.y;

				if (collisionData[i].V.z > biggestZ)
					biggestZ = collisionData[i].V.z;
			}

			// update the AABB with the new data
			m_AABB = AABB3(glm::vec3(smallestX, smallestY, smallestZ), glm::vec3(biggestX, biggestY, biggestZ));

			// clear vector as no longer required
			collisionData.clear();

			// genereate a buffer
			glGenBuffers(1, &m_vbo);
			glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertNormalUV)*m_vertexData.size(), &m_vertexData[0], GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			// genereate a buffer
			glGenBuffers(1, &m_ibo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*m_indices.size(), &m_indices[0], GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			std::cout << "LOADED\n";
		}
		else
		{
			std::cout << "FAILED\n";
		}
	}
}
Object::~Object()
{
	// clear vectors
	m_vertexData.clear();
	m_indices.clear();
	// delete buffers
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ibo);
}
void Object::draw()
{
	// enable vertex data and indices data arrays
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);

	//Const foat pointer that points to a offset of NULL
	const float* coordinate = 0;
	// enable vertex array
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(vertNormalUV), coordinate);
	glEnableVertexAttribArray(0);

	// enable normal
	glVertexAttribPointer(1, 3, GL_FLOAT, true, sizeof(vertNormalUV), coordinate + 3);
	glEnableVertexAttribArray(1);

	// enable texture
	glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(vertNormalUV), coordinate + 6);
	glEnableVertexAttribArray(2);

	// draw the indexed triangles from the VBO
	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);

	// disable editing of array
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	// empty buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
glm::vec3 Object::getTranslation()
{
	glm::vec3 translation;
	translation.x = m_matrix[3].x;
	translation.y = m_matrix[3].y;
	translation.z = m_matrix[3].z;

	return translation;
}
void Object::rotate(float x, float y)
{
	m_matrix = glm::rotate(m_matrix, glm::radians(x), m_dirX); // vertical
	m_matrix = glm::rotate(m_matrix, glm::radians(y), m_dirY); // horizontal

	glm::vec3 scale;
	scale.x = m_matrix[3].x;
	scale.y = m_matrix[3].y;
	scale.z = m_matrix[3].z;

	std::cout << m_AABB.min.x << " " << m_AABB.min.y << " " << m_AABB.min.z << std::endl;
	std::cout << m_AABB.max.x << " " << m_AABB.max.y << " " << m_AABB.max.z << std::endl;

	glm::vec3 minScaled = glm::rotate(m_AABB.center, glm::radians(y), m_dirY);
	glm::vec3 maxScaled = glm::rotate(m_AABB.max, glm::radians(y), m_dirY);

	std::cout << minScaled.x << " " << minScaled.y << " " << minScaled.z << std::endl;
	std::cout << maxScaled.x << " " << maxScaled.y << " " << maxScaled.z << std::endl;

	m_AABB = AABB3(minScaled, maxScaled);
}
void Object::translate(glm::vec3 vector)
{
	m_matrix[3] = glm::vec4(vector, 1.0f);
	m_AABB = AABB3((m_AABB.min + vector), (m_AABB.max + vector));
}
void Object::scale(glm::mat4 matrix)
{ 
	m_matrix *= matrix; 

	// scale the AABB
	glm::vec3 scale;
	scale.x = matrix[0].x;
	scale.y = matrix[1].y;
	scale.z = matrix[2].z;

	m_AABB = AABB3((m_AABB.min * scale), (m_AABB.max * scale));
}
