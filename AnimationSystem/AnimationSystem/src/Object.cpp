// Author  : Lewis Ward
// Program : Animation System Tech Demo
// Date    : 18/04/2015
#include "Object.h"

Object::Object(const char* meshFile)
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
}
