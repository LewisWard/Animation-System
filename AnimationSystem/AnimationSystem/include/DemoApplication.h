// Author  : Lewis Ward
// Program : Animation System Tech Demo
// Date    : 12/02/2015

/*
	This program will allow for users to export a Maya animation with the custom Maya exporter.
	Maya plugin crash when no Trajectory or Hip joint is within the scene.
	!IT IS HIGHLY RECOMMENDED to use a XBOX 360 CONTROLLER for moving around the scene!
*/

#pragma once
#include "Mesh.h"
#include "Menu.h"
#include "Object.h"
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

	/// \brief updates the array of movement key are being pressed
	void keybaordMovementUpdate();

	Window m_window; ///< demo window
	Event m_events;  ///< demo user input events
	Mesh* m_mesh[ANIMCOUNT]; ///< the animation cycles, joint clusters etc.
	animCycle m_currentState; ///< the current state the animation is in (i.e. Idle or Walking)
	float m_currentFrame; ///< current frame
	gls::Program* m_program; ///< shader program
	gls::Program* m_objects; ///< shader program
	gls::Program* m_menuProgram; ///< shader program
	std::shared_ptr<Camera> m_camera; ///< camera
	int m_eventCode; ///< input event codes
	XboxController m_controller; ///< Xbox 360 Controller
	glm::mat4 m_trajectoryJoint; ///< skeleton m_trajectory joint
	std::vector<Object*> m_object; ///< non-animating 3D models (i.e. buildings)
	Texture* m_texture; ///< texture
	Texture* m_exoTexture; ///< texture
	Texture* m_bigTexture; ///< texture
	Texture* m_shopTexture; ///< texture
	Texture* m_houseTexture; ///< texture
	Texture* m_wallTexture; ///< texture 
	Texture* m_mainMenuTexture; ///< texture
	Texture* m_contMenuTexture; ///< texture
	bool* m_movement; ///< array of current mouse/keyboard inputs
	int m_state; ///< state the program is in
	Menu* m_menu; ///< menu system
};
