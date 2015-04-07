// Author  : Lewis Ward
// Program : Animation System Tech Demo
// Date    : 12/02/2015

/*
	This program will allow for users to export a Maya animation with the custom Maya exporter.

	Feature list:
		Display animation  cycle.
		Perform animation  blending between different cycles (i.e walking into jogging and running)
		Allow user to move camera around the scene.
		Allow user input to control character animation.
		Allow input from Xbox Controller.

	Fix list:
		Maya plugin crash when no Trajectory, and I guess same for Hip, joint is within the scene.
*/

#pragma once
#include "Mesh.h"
#include "Camera.h"
#include "gl/Program.h"
#include <memory>

#define ANIMPATH "./animations/"
#define ANIMCOUNT 2

enum animCycle
{
	Idle,
	Walk,
};

//----------------------------------------------------------------------------------------------------------------------
/// \brief  The application, alomst everything is called or run from within this class
//----------------------------------------------------------------------------------------------------------------------
class Application
{
public:
	/// \brief Constructor
	Application();
	/// \brief Destructor
	~Application();

	/// \brief draw everything that needs to be drawn
	void draw();

	/// \brief update everything 
	/// \prama float delta time
	void update(float dt);

	/// \brief get the state of the gamellop
	/// \return bool quit gameloop if false
	inline bool getGameloop()
	{ return m_events.playState(); }

private:
	Window m_window; ///< demo window
	Event m_events;  ///< demo user input events
	Mesh* m_mesh[ANIMCOUNT];
	animCycle m_currentState;
	float m_currentFrame;
	gls::Program* m_program;
	std::shared_ptr<Camera> m_camera;
	int m_eventCode;
	XboxController m_controller;
	glm::mat4 m_trajectoryJoint;
};
