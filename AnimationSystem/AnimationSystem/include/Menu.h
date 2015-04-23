// Author  : Lewis Ward
// Program : Animation System Tech Demo
// Date    : 23/04/2015
#pragma once
#include "Mesh.h"

//----------------------------------------------------------------------------------------------------------------------
/// \brief	Menu system
//----------------------------------------------------------------------------------------------------------------------
class Menu
{
public:

	Menu();
	~Menu();

	void draw();

	void addButton(AABB2 aabb);

	int buttonClicked(glm::vec2 mousePos);

private:
	std::vector<AABB2> m_buttons;
	uint32_t m_vbo;
	uint32_t m_ibo;
};
