// Author  : Lewis Ward
// Program : Animation System Tech Demo
// Date    : 12/02/2015
#include "DemoApplication.h"

#define ANIMPATH "./animations/"

Application::Application()
{
	m_mesh = std::make_shared<Mesh>(ANIMPATH"bodyBig2.amesh");

	m_currentFrame = 0.0f;
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


	glm::vec3 mat;

	// get the Trajectory joint
	glm::vec3 vec = m_mesh.get()->getMesh().deformed[m_mesh.get()->getMesh().deformed.size() - NUM_OF_FRAMES].V;
	vec.x += 100.0f;
	vec.y += 400.0f;
	vec.z = 1.0f;

	// draw joints
	for (size_t i = 0; i < m_mesh.get()->getMesh().deformed.size() - NUM_OF_FRAMES; i += NUM_OF_FRAMES)
	{
		mat = m_mesh.get()->getMesh().deformed[i + m_currentFrame].V;

		mat.x += 100.0f;
		mat.y += 400.0f;
		mat.z = 1.0f;

		glBegin(GL_POINTS);
			glColor3f(1.0f, 1.0f, 0.0f);
			glVertex3f(mat.x, mat.y, mat.z);
			glColor3f(0.0f, 1.0f, 0.0f);
			glVertex3f(vec.x, vec.y, vec.z);
		glEnd();

		std::cout << m_currentFrame << " " << mat.x << " " << mat.y << " " << mat.z << std::endl;
	}
	

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

	// go to the next frame
	m_currentFrame += dt * NUM_OF_FRAMES;

	// reset the frame
	if (m_currentFrame > NUM_OF_FRAMES)
		m_currentFrame = 0.0f;
}
