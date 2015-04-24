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
	/// \brief Constructor
	Menu();

	/// \brief Destructor
	~Menu();

	/// \brief draw the menu
	void draw();

	/// \brief add a button to the menu
	/// \prama AABB2 button AABB
	void addButton(AABB2 aabb);

	/// \brief test to see if a button has been clicked on
	/// \prama glm::vec2 position of the mouse
	/// \return button index
	int buttonClicked(glm::vec2 mousePos);

private:
	std::vector<AABB2> m_buttons; ///< all the menus this menu has
	uint32_t m_vbo; ///< VBO
	uint32_t m_ibo; ///< IBO
};
