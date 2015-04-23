// Author  : Lewis Ward
// Program : Animation System Tech Demo
// Date    : 12/02/2015
#include "DemoApplication.h"

#define ANIMPATH "./animations/"

Application::Application()
{
	m_eventCode = 0;
	m_currentState = Idle;

	 m_movement = new bool[3]{
		false, // forward 
		false, // moveLeft
		false  // moveRight
	};

	// setup GLEW, if falis quit application
	if (!initGLEW())
	{
		m_events.forceQuit();
	}
	else
	{
		m_mesh[0] = new Mesh(ANIMPATH"ExoIdle.amesh", "meshes/ExoCollision.meshes");
		m_mesh[1] = new Mesh(ANIMPATH"ExoWalk.amesh", "meshes/ExoCollision.meshes");

		m_object.resize(2);
		m_object[0] = new Object("meshes/Building_Shop.meshes", "meshes/Building_ShopCollision.meshes");
		m_object[1] = new Object("meshes/Floor.meshes", "meshes/Floor.meshes");

		m_texture = new Texture("images/Floor.png");
		m_exoTexture = new Texture("images/Exo.png");

		// scale and rotate the objects
		glm::mat4 scale;
		scale[0].x = 5;
		scale[1].y = 5;
		scale[2].z = 5;
		//m_object[0]->scale(scale);
		scale[0].x = 15;
		scale[1].y = 15;
		scale[2].z = 15;
		m_object[1]->scale(scale);
		//m_object[0]->rotate(0.0f, 90.0f);
		//m_object[0]->translate(glm::vec3(10.0f, 0.0f, -45.0f));

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

	delete[] m_movement;
	delete[] m_mesh;

	for (int i = 0; i < m_object.size(); ++i)
	{
		delete m_object[i];
		m_object[i] = nullptr;
	}

	m_object.clear();
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

		// recompute MV/MVP matrix
		MV = V * m_object[0]->matrix();
		MVP = P * MV;

		// set shader uniforms
		m_objects->uniform_Matrix4("mvp", 1, false, MVP);
		m_objects->uniform_Matrix4("mv", 1, false, MV);
		m_objects->uniform_1i("texture", 0);
		m_objects->uniform_4f("specular", 0.0f, 0.0f, 0.15f, 1.0f);
		
		// draw
		m_object[0]->draw();


		// recompute MV/MVP matrix
		MV = V * m_object[1]->matrix();
		MVP = P * MV;
		m_objects->uniform_Matrix4("mvp", 1, false, MVP);
		m_objects->uniform_Matrix4("mv", 1, false, MV);
		m_object[1]->draw();


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

	// update keybaord and Xbox controller
	keybaordMovementUpdate();
	m_controller.update(dt);

	animCycle lastState = m_currentState;

	if (fabs(m_controller.getRightStick().y) > 0.0f || m_movement[0] || m_movement[1] || m_movement[2])
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

	// if the state has changed update the direction the mech is facing for other state
	if(lastState != m_currentState)
	{
		if (m_currentState == Walk)
			m_mesh[Walk]->hAngle(m_mesh[Idle]->getHAngle());
		else
			m_mesh[Idle]->hAngle(m_mesh[Walk]->getHAngle());
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

	m_mesh[m_currentState]->update(dt, m_currentFrame, m_events, m_movement, m_controller);

	// get the Trajectory joint position
	m_trajectoryJoint = m_mesh[m_currentState]->getModelMatrix();

	std::cout << m_trajectoryJoint[3].x << " " << m_trajectoryJoint[3].y << " " << m_trajectoryJoint[3].z << std::endl;
	
	m_camera->update(dt, m_events, m_controller, m_trajectoryJoint);

	glm::vec3 collision;
	collision.x = m_trajectoryJoint[3].x;
	collision.y = m_trajectoryJoint[3].y;
	collision.z = m_trajectoryJoint[3].z;

	if (m_object[0]->getAABB().intersect(m_mesh[m_currentState]->getAABB()))
	{
		std::cout << "HIT\n";
	}

	// go to the next frame
	m_currentFrame += dt * NUM_OF_FRAMES;

	// reset the frame
	if (m_currentFrame > NUM_OF_FRAMES)
		m_currentFrame = 0.0f;
}
void Application::keybaordMovementUpdate()
{
		if (m_eventCode == kWdown)
		{
			m_movement[0] = true;
			#ifdef _DEBUG 
				std::cout << "kWdown move: true "<< std::endl; 
			#endif
		}
		else if (m_eventCode == kWUp)
		{
			m_movement[0] = false;
			#ifdef _DEBUG 
				std::cout << "kWUp move: false " << std::endl;
			#endif
		}

		if (m_eventCode == kAdown)
		{
			m_movement[1] = true;
			#ifdef _DEBUG 
				std::cout << "kAdown move: true " << std::endl;
			#endif
		}
		else if (m_eventCode == kAUp)
		{
			m_movement[1] = false;
			#ifdef _DEBUG 
				std::cout << "kAUp move: false " << std::endl;
			#endif
		}
		else if (m_eventCode == kDdown)
		{
			m_movement[2] = true;
			#ifdef _DEBUG 
				std::cout << "kDdown move: true " << std::endl;
			#endif
		}
		else if (m_eventCode == kDUp)
		{
			m_movement[2] = false;
			#ifdef _DEBUG 
				std::cout << "kDUp move: false " << std::endl;
			#endif
		}
}