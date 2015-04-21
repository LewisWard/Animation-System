// Author  : Lewis Ward
// Program : Animation System Tech Demo
// Date    : 12/02/2015
#include "DemoApplication.h"

#define ANIMPATH "./animations/"

Application::Application()
{
	m_eventCode = 0;
	m_currentState = Idle;

	// setup GLEW, if falis quit application
	if (!initGLEW())
	{
		m_events.forceQuit();
	}
	else
	{
		m_mesh[0] = new Mesh(ANIMPATH"testIdle.amesh");
		m_mesh[1] = new Mesh(ANIMPATH"testWalk.amesh");

		m_object = new Object("meshes/example.meshes");

		m_texture = new Texture("images/example.png");
		m_exoTexture = new Texture("images/Exo.png");

		// scale and rotate the object
		glm::mat4 scale;
		scale[0].x = 2;
		scale[1].y = 2;
		scale[2].z = 2;
		m_object->scale(scale);
		m_object->rotate(0.0f, -90.0f);

		m_camera = std::make_shared<Camera>(m_window.width(), m_window.height());

		m_currentFrame = 0.0f;

		m_program = new gls::Program();
		m_objects = new gls::Program();

		// create an array of all shaders to load
		gls::Shader shaders[] = {
			gls::Shader("shaders/joint.vtx.glsl", gls::sVERTEX), ///< joint vertex shader
			gls::Shader("shaders/joint.pix.glsl", gls::sFRAGMENT), ///< joint fragment shader
			gls::Shader("shaders/object.vtx.glsl", gls::sVERTEX), ///< object vertex shader
			gls::Shader("shaders/object.pix.glsl", gls::sFRAGMENT), ///< object fragment shader
		};
		
		// create shader programs
		m_program->create(&shaders[0], &shaders[1]);
		m_objects->create(&shaders[2], &shaders[3]);
	}
}
Application::~Application()
{
	delete m_texture;
	delete m_exoTexture;
	delete m_program;
	delete m_objects;
	m_program = nullptr;
	m_objects = nullptr;

	delete[] m_mesh;
	delete m_object;
	m_object = nullptr;
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
	glm::mat4 model = m_mesh[m_currentState]->getModelMatrix();

	// compute the Model-View-Project Matrix
	glm::mat4x4 MV = V * model;
	MVP = P * MV;

	//// bind the program
	//m_program->bind();
	//// shader unifroms
	//m_program->uniform_Matrix4("mvp", 1, false, MVP);
	//m_program->uniform_Matrix4("mv", 1, false, MV);
	//	// draw the Trajectory joint
	//	//m_mesh[m_currentState]->draw(0);
	//	//m_mesh[m_currentState]->drawObject();
	//	for (size_t i = m_currentFrame; i < m_currentFrame * m_mesh[m_currentState]->numberOfFrames(); i += m_mesh[m_currentState]->numberOfFrames())
	//	{
	//		//m_mesh[m_currentState]->draw(i);
	//		//m_mesh[m_currentState]->drawObject(i);
	//	}
	//// unbind program and texture
	//m_program->unbind();



	// bind program
	m_objects->bind();
		//bind texture
		m_texture->bind(0);
		m_exoTexture->bind(1);

		m_objects->uniform_Matrix4("mvp", 1, false, MVP);
		m_objects->uniform_Matrix4("mv", 1, false, MV);
		m_objects->uniform_1i("texture", 1);
		m_objects->uniform_4f("ambient", 0.0f, 0.0f, 0.3f, 1.0f);
		m_objects->uniform_4f("diffuse", 1.0f, 1.0f, 1.0f, 1.0f);
		m_objects->uniform_4f("specular", 0.5f, 0.5f, 0.55f, 1.0f);

		m_mesh[m_currentState]->drawObject();
		for (size_t i = m_currentFrame; i < m_currentFrame * m_mesh[m_currentState]->numberOfFrames(); i += m_mesh[m_currentState]->numberOfFrames())
			m_mesh[m_currentState]->drawObject(i);


		// recompute MV/MVP matrix
		MV = V * m_object->matrix();;
		MVP = P * MV;

		// set shader uniforms
		m_objects->uniform_Matrix4("mvp", 1, false, MVP);
		m_objects->uniform_Matrix4("mv", 1, false, MV);
		m_objects->uniform_1i("texture", 0);
		m_objects->uniform_4f("specular", 0.0f, 0.0f, 0.15f, 1.0f);
		
		// draw
		m_object->draw();

		// unbind texture
		m_texture->unbind();
		m_exoTexture->unbind();

	// unbind program
	m_objects->unbind();
	

	// disable OpenGL textures and depth testing
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);

	// swap the SDL window
	SDL_GL_SwapWindow(m_window.getWindow());
}
void Application::update(float dt)
{
	// get lastest event 
	m_eventCode = m_events.update();

	m_controller.update(dt);

	if (fabs(m_controller.getRightStick().x) > 0.75f)
	{
		// change state and update next animation cycle trajectory poisiton
		m_currentState = Walk;
		m_mesh[m_currentState]->setModelMatrix(m_trajectoryJoint);
	}
	else
	{
		// change state and update next animation cycle trajectory poisiton
		m_currentState = Idle;
		m_mesh[m_currentState]->setModelMatrix(m_trajectoryJoint);
	}

	// Debug builds only
	#ifdef _DEBUG
		// turn on fill
		if (m_controller.getLastButtonPressed() == kX)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		// turn on wirefire
		if (m_controller.getLastButtonPressed() == kY)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}

		if (m_controller.getLastButtonPressed() == kA)
		{
			// change state and update next animation cycle trajectory poisiton
			m_currentState = Walk;
			m_mesh[m_currentState]->setModelMatrix(m_trajectoryJoint);
	
		}
		else 	if (m_controller.getLastButtonPressed() == kB)
		{
			// change state and update next animation cycle trajectory poisiton
			m_currentState = Idle;
			m_mesh[m_currentState]->setModelMatrix(m_trajectoryJoint);
		}
	#endif

	m_mesh[m_currentState]->update(dt, m_currentFrame, m_events, m_controller);
	
	// get the Trajectory joint position
	m_trajectoryJoint = m_mesh[m_currentState]->getModelMatrix();
	glm::vec3 mVector;
	mVector.x = m_trajectoryJoint[3].x;
	mVector.y = m_trajectoryJoint[3].y;
	mVector.z = m_trajectoryJoint[3].z;
	
	m_camera->update(dt, m_events, m_controller, mVector);

	// go to the next frame
	m_currentFrame += dt * NUM_OF_FRAMES;

	// reset the frame
	if (m_currentFrame > NUM_OF_FRAMES)
		m_currentFrame = 0.0f;
}
