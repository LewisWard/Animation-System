// Author  : Lewis Ward
// Program : Animation System Tech Demo
// Date    : 13/02/2015
#pragma once
#include "Window.h"

// This .h file has been taken but modified from my PGAG Assignment 1 project

// all the events that I require
enum events
{
	kNONE, ///< nothing pressed
	kSpace, ///< Space bar 
	kMDL, kMDM, kMDR, ///< Mouse Down Left/Middle/Right
	kCtrl, kAlt ///< left control / left Alt
};

//----------------------------------------------------------------------------------------------------------------------
/// \brief  Handles all input events
//----------------------------------------------------------------------------------------------------------------------
class Event
{
public:
	///\brief Constructor
	Event();
	///\brief Destructor
	~Event();

	///\brief update events
	///\return int the event code (see events enum)
	int update();

	///\brief get the play state
	///\prama bool false to quit program
	inline bool playState()
	{ return m_playState; }

	///\brief force quit the program
	inline void forceQuit()
	{ m_playState = false; }

	///\brief update mouse position
	///\return math::vec2 new mouse position
	inline glm::vec2 mouseUpdate()
	{ return m_mouse; }

private:
	SDL_Event m_event; ///< SDL event
	glm::vec2 m_mouse; ///< mouse position
	bool m_playState; ///< keep gameloop going
};
