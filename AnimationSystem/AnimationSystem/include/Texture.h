// Author : Lewis Ward (http://www.lewis-ward.com)
// Program : Animation System Tech Demo
// Date    : 19/02/2015
// © 2015-2016 Lewis Ward. All rights reserved.
#pragma once
#include "Window.h"

// This header file has been taken but modified from my PGAG Assignment 1 project

//----------------------------------------------------------------------------------------------------------------------
/// \brief  Texture class
//----------------------------------------------------------------------------------------------------------------------
class Texture
{
public:
	///\brief Constructor
	///\prama const char* texture file name
	Texture(const char* texture);
	///\brief Destructor
	~Texture();
	
	///\brief bind the texture
	void bind(int slot);

	///\brief unbind the texture
	void unbind();

	///\brief get the texture
	///\return uint32_t
	inline uint32_t texture()
	{ return m_texture; }

private:
	uint32_t m_texture; ///< texture
};
