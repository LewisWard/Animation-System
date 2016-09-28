// Author : Lewis Ward (http://www.lewis-ward.com)
// Program : Animation System Tech Demo
// Date    : 06/04/2015
// © 2015-2016 Lewis Ward. All rights reserved.
#pragma once
#include "Window.h"
#include <Windows.h>
#include <Xinput.h>

enum XboxButtons
{
	kNone,
	kA, kB, kX, kY,
	kLS, kRS,
};

//----------------------------------------------------------------------------------------------------------------------
/// \brief Handles a xbox controller
//----------------------------------------------------------------------------------------------------------------------
class XboxController
{
public:
	/// \brief Constructor
	XboxController();
	/// \brief Destructor
	~XboxController();

	/// \brief update the camera
	/// \prama float delta time
	void update(float dt);

	/// \brief check if the controller is connected
	/// \return bool true if connected
	inline bool isConnectedState()
	{ return m_connected; }

	/// \brief get the left stick movement value
	/// \return glm::vec2 left stick
	inline glm::vec2 getLeftStick()
	{ return m_leftStick; }

	/// \brief get the right stick movement value
	/// \return glm::vec2 right stick
	inline glm::vec2 getRightStick()
	{ return m_rightStick; }

	XboxButtons getLastButtonPressed();

	inline float getLeftTrigger()
	{ return m_leftTrigger; }

	inline float getRightTrigger()
	{ return m_rightTrigger; }

private:
	DWORD m_dwReulst; ///< used to get result from XINPUT
	XINPUT_STATE m_state; ///< the state of the controller
	bool m_connected; ///< controller connected state
	glm::vec2 m_leftStick; ///< left stick movement value
	glm::vec2 m_rightStick; ///< right stick movement value
	XboxButtons m_buttonPressed; ///< the button on controller
	float m_leftTrigger; ///< left trigger value
	float m_rightTrigger; ///< right trigger value
};
