// Author  : Lewis Ward
// Program : Animation System Tech Demo
// Date    : 12/02/2015

/*
	This program will allow for users to export a Maya animation with the custom Maya exporter.

	Feature list:
		Display anaimation cycle.
		Perform anaiamtion blending between different cycles (i.e walking into jogging and running)
		Allow user to move camera around the sence.
		Allow user input to control character animation.
		Allow input from Xbox Controller
*/

#pragma once
#include "Events.h"
#include "gl/Program.h"
#include "Mesh.h"
#include <memory>

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
	std::shared_ptr<Mesh> m_mesh;
	float m_currentFrame;
};
