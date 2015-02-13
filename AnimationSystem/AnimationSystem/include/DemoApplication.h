// Author  : Lewis Ward
// Program : Animation System Tech Demo
// Date    : 12/02/2015
#pragma once
#include "Events.h"
#include "gl/Program.h"
#include <memory>

//----------------------------------------------------------------------------------------------------------------------
/// \brief  The application 
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
};
