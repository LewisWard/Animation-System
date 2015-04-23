// Author  : Lewis Ward
// Program : Animation System Tech Demo
// Date    : 23/04/2015
#pragma once
#include "Menu.h"

Menu::Menu()
{
	// vertex array
	vertUV vertices[4];
	// set vertex data
	vertices[0].V.x = -1.0f;
	vertices[0].V.y = 1.0f;
	vertices[0].V.z = 0.0f;
	vertices[0].U.x = 0.0f;
	vertices[0].U.y = 0.0f;
	vertices[1].V.x = 1.0f;
	vertices[1].V.y = 1.0f;
	vertices[1].V.z = 0.0f;
	vertices[1].U.x = 1.0f;
	vertices[1].U.y = 0.0f;
	vertices[2].V.x = -1.0f;
	vertices[2].V.y = -1.0f;
	vertices[2].V.z = 0.0f;
	vertices[2].U.x = 0.0f;
	vertices[2].U.y = 1.0f;
	vertices[3].V.x = 1.0f;
	vertices[3].V.y = -1.0f;
	vertices[3].V.z = 0.0f;
	vertices[3].U.x = 1.0f;
	vertices[3].U.y = 1.0f;
	// indices
	int indices[] =
	{
		0, 1, 2,
		2, 3, 1
	};

	// genereate a buffer
	glGenBuffers(1, &m_vbo);
	// bind
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertUV)* 4, &vertices[0], GL_STATIC_DRAW);
	// unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// genereate a buffer
	glGenBuffers(1, &m_ibo);
	// bind
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)* 6, &indices[0], GL_STATIC_DRAW);
	// unbind
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
Menu::~Menu()
{
	m_buttons.clear();

	// delete buffers
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ibo);
}
void Menu::addButton(AABB2 aabb)
{
	m_buttons.push_back(aabb);
}
int Menu::buttonClicked(glm::vec2 mousePos)
{
	// cycle buttons to see if user has clicked on one
	for (size_t i = 0; i < m_buttons.size(); ++i)
	{
		if (m_buttons[i].contains(mousePos))
			return i;
	}

	return -1;
}
void Menu::draw()
{
	// bind vbo and ibo buffers
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);

	// an offset from NULL
	const float* offset = 0;
	// enable vertex
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(vertUV), offset);
	glEnableVertexAttribArray(0);

	// enable (uv)
	glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(vertUV), offset + 3);
	glEnableVertexAttribArray(1);

	// draw
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	// disable from modification
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	// unbind buffers
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
