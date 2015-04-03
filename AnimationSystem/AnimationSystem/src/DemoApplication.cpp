// Author  : Lewis Ward
// Program : Animation System Tech Demo
// Date    : 12/02/2015
#include "DemoApplication.h"

#define ANIMPATH "./animations/"

Application::Application()
{
	// setup GLEW, if falis quit application
	if (!initGLEW())
	{
		m_events.forceQuit();
	}
	else
	{
		m_mesh = std::make_shared<Mesh>(ANIMPATH"bodyBig2.amesh");

		m_camera = std::make_shared<Camera>(m_window.width(), m_window.height());

		m_currentFrame = 0.0f;

		m_program = new gls::Program();

		// create an array of all shaders to load
		gls::Shader shaders[] = {
			gls::Shader("shaders/joint.vtx.glsl", gls::sVERTEX), ///< joint
			gls::Shader("shaders/joint.pix.glsl", gls::sFRAGMENT), ///< joint
		};
		
		m_program->create(&shaders[0], &shaders[1]);
	}
}
Application::~Application()
{
	delete m_program;
	m_program = nullptr;
}
void Application::draw()
{
	// enable OpenGL 2D textures and depth testing
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);

	// clear the colour and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// get the MVP martix
	glm::mat4x4 MVP;
	glm::mat4x4 V = m_camera.get()->viewMatrix();
	glm::mat4x4 P = m_camera.get()->projectionMatrix();
	glm::mat4 m_model;
	m_model[3].x = -2.0f;

	glm::mat4x4 MV = V * m_model;
	MVP = P * MV;

	std::printf("%4.2f %4.2f %4.2f %4.2f \n%4.2f %4.2f %4.2f %4.2f \n%4.2f %4.2f %4.2f %4.2f \n%4.2f %4.2f %4.2f %4.2f\n", 
							MVP[0].x, MVP[0].y, MVP[0].z, MVP[0].w,
							MVP[1].x, MVP[1].y, MVP[1].z, MVP[1].w,
							MVP[2].x, MVP[2].y, MVP[2].z, MVP[2].w,
							MVP[3].x, MVP[3].y, MVP[3].z, MVP[3].w);

	// bind the program
	m_program->bind();
		// shader unifroms
	m_program->uniform_Matrix4("mvp", 1, false, MVP);
		// draw the menu
		for (size_t i = 0; i < m_mesh.get()->getMesh().deformed.size() - NUM_OF_FRAMES; i += NUM_OF_FRAMES)
		m_mesh->draw(i);
	// unbind program and texture
	m_program->unbind();

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

		//std::cout << m_currentFrame << " " << mat.x << " " << mat.y << " " << mat.z << std::endl;
	}
	

	// disable OpenGL textures and depth testing
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);

	// swap the SDL window
	SDL_GL_SwapWindow(m_window.getWindow());
}
void Application::update(float dt)
{
	m_camera->update(dt, m_events);
	//m_camera.get()->update(dt, m_events);

	// get lastest event 
	int eventCode = m_events.update();

	// go to the next frame
	m_currentFrame += dt * NUM_OF_FRAMES;

	// reset the frame
	if (m_currentFrame > NUM_OF_FRAMES)
		m_currentFrame = 0.0f;
}
