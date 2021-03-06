// Author : Lewis Ward (http://www.lewis-ward.com)
// Program : Animation System Tech Demo
// Date    : 13/02/2015
// � 2015-2016 Lewis Ward. All rights reserved.
#include "Events.h"

// This .cpp file has been taken but modified from my PGAG Assignment 1 project
Event::Event()
{
	m_playState = true;
}
Event::~Event()
{

}
int Event::update()
{
	// process SDL events
	while(SDL_PollEvent(&m_event))
	{
		switch(m_event.type)
		{
		case SDL_WINDOWEVENT:
			break;
		case SDL_KEYDOWN:
				if (SDLK_ESCAPE == m_event.key.keysym.sym)
				{ m_playState = false; } ///< quit window/program
				if (SDLK_w == m_event.key.keysym.sym)
				{ return kWdown; }
				else if (SDLK_s == m_event.key.keysym.sym)
				{ return kSdown; }
				if (SDLK_a == m_event.key.keysym.sym)
				{	return kAdown; }
				else if(SDLK_d == m_event.key.keysym.sym)
				{ return kDdown; }
				else if (SDLK_q == m_event.key.keysym.sym)
				{ return kQdown; }
				else if (SDLK_e == m_event.key.keysym.sym)
				{ return kEdown; }
				if(SDLK_SPACE == m_event.key.keysym.sym)
				{ return kSpace; }
				if (SDLK_LCTRL == m_event.key.keysym.sym)
				{ return kCtrl; }
				if (SDLK_LALT == m_event.key.keysym.sym)
				{ return kAlt; }
				// key that I'm not worried about has been pressed
				return kNONE;
			break;
		case SDL_KEYUP:
			if (SDLK_w == m_event.key.keysym.sym)
			{ return kWUp; }
			else if (SDLK_s == m_event.key.keysym.sym)
			{ return kSUp; }
			if (SDLK_a == m_event.key.keysym.sym)
			{ return kAUp; }
			else if (SDLK_d == m_event.key.keysym.sym)
			{ return kDUp; }
			else if (SDLK_q == m_event.key.keysym.sym)
			{ return kQUp; }
			else if (SDLK_e == m_event.key.keysym.sym)
			{ return kEUp; }
			if (SDLK_SPACE == m_event.key.keysym.sym)
			{ return kSpaceUp; }
			break;
		case SDL_MOUSEMOTION:
			// update mouse position
			m_mouse.x = m_event.motion.x;
			m_mouse.y = m_event.motion.y;
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (SDL_BUTTON_LEFT == m_event.button.button)
			{ return kMDL; }
			if (SDL_BUTTON_MIDDLE == m_event.button.button)
			{ return kMDM; }
			if (SDL_BUTTON_RIGHT == m_event.button.button)
			{ return kMDR; }
			break;
		case SDL_QUIT: m_playState = false; ///< quit window/program
			break;
		}
	}
}
