// Author  : Lewis Ward
// Program : Animation System Tech Demo
// Date    : 12/02/2015
#include "DemoApplication.h"

Application::Application()
{

}
Application::~Application()
{

}
void Application::draw()
{
	// enable OpenGL 2D textures and depth testing
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);

	// clear the colour and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	// disable OpenGL textures and depth testing
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);

	// swap the SDL window
	SDL_GL_SwapWindow(m_window.getWindow());
}
void Application::update(float dt)
{
	// get lastest event 
	int eventCode = m_events.update();
}
