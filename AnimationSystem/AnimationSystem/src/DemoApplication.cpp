// Author  : Lewis Ward
// Program : Animation System Tech Demo
// Date    : 12/02/2015
#include "DemoApplication.h"

Application::Application()
{
	//Mesh mesh("./animations/joint.amesh");

	m_mesh = std::make_shared<Mesh>("./animations/basicBody.amesh");
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

	// draw the joint
	glm::mat4x3 mat = m_mesh.get()->getMesh().clusters[0].bindPose;
	mat[3][0] += 100.0f;
	mat[3][1] += 100.0f;
	glBegin(GL_POINTS);
		glVertex3f(mat[3][0], mat[3][1], mat[3][2]);
	glEnd();

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
