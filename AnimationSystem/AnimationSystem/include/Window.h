// Author  : Lewis Ward
// Program : Animation System Tech Demo
// Date    : 09/02/2015
#pragma once
#define GLEW_STATIC
#include "gl/glew.h"
#include "SDL.h"
#include "SDL_opengl.h"
#include "SDL_image.h"
#include "glm.hpp"
#include "gtc/type_ptr.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtx/transform.hpp"
#include "gtx/compatibility.hpp"
#include "gtx/quaternion.hpp"
#include "gtx/rotate_vector.hpp"
#include <iostream>
#include <Windows.h>
#include <cstdint>

// This header file has been taken but modified from my PGAG Assignment 1 project

//----------------------------------------------------------------------------------------------------------------------
/// \brief  SDL and GLEW initialisation
/// \return bool true if SDL and GLEW initialisation was susscessful
//----------------------------------------------------------------------------------------------------------------------
bool initSDL();
bool initGLEW();

//----------------------------------------------------------------------------------------------------------------------
/// \brief  Delta Time 
//----------------------------------------------------------------------------------------------------------------------
void initTimerDT(); ///< initialisation
float updateTimerDT(); ///< update, return delta time

//----------------------------------------------------------------------------------------------------------------------
/// \brief  SDL/OpenGL Window
//----------------------------------------------------------------------------------------------------------------------
class Window 
{
public:
	///\brief Constructor
	Window();
	///\brief Destructor
	~Window();

	///\brief set the title of the window
	///\prama const char* new name for thw window
	void setTitle(const char* title);

	///\brief set the icon of the window
	///\prama SDL_Surface* add a icon to the window
	void setIcon(SDL_Surface* icon);

	///\brief get the window pointer
	///\return SDL_Window*
	inline SDL_Window* getWindow()
	{ return m_windowSDL; }

	inline int width()
	{ return m_winW; }

	inline int height()
	{ return m_winH; }

private:
	int m_winX; ///< x position
	int m_winY; ///< y position
	int m_winW; ///< window width
	int m_winH; ///< window height
	SDL_Window* m_windowSDL; ///< SDL window
	SDL_GLContext m_windowGL; ///< OpenGL window handle
};
