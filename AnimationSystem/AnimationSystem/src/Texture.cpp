// Author  : Lewis Ward
// Program : Animation System Tech Demo
// Date    : 19/02/2015
#include "Texture.h"

// This cpp file has been taken but modified from my PGAG Assignment 1 project

Texture::Texture(const char* texture)
{
	// most of this code for converting a SDL_Surface to glTexture is based upon link below: 
	// http://www.sdltutorials.com/sdl-tip-sdl-surface-to-opengl-texture

	// create a surface from loading texture
	SDL_Surface* imageSurface = IMG_Load(texture);

	// if the image was loaded
	if (imageSurface)
	{
		// generate a texture
		glGenTextures(1, &m_texture);
		// bind texture
		glBindTexture(GL_TEXTURE_2D, m_texture);
		
		// select the right texture data for texture
		if (imageSurface->format->BytesPerPixel == 4)
		{
			// texture data (PNG with Alpha channel)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageSurface->w, imageSurface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageSurface->pixels);
		}
		else
		{
			std::cout << "Incorrect BPP format\n";
		}

		// texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// unbind texture
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	
	// delete the surface, no longer required
	SDL_FreeSurface(imageSurface);
}

Texture::~Texture()
{
	// delete textures
	glDeleteTextures(1, &m_texture);
}
void Texture::bind(int slot)
{
	// bind the buffer
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_texture);
}
void Texture::unbind()
{
	// unbind buffer
	glBindTexture(GL_TEXTURE_2D, 0);
}
