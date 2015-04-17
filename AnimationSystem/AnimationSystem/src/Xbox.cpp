// Author  : Lewis Ward
// Program : Animation System Tech Demo
// Date    : 06/04/2015
#include "Xbox.h"

XboxController::XboxController()
{
	// set to false
	m_connected = false;

	// zero the state
	ZeroMemory(&m_state, sizeof(XINPUT_STATE));

	// get the state of the controller
	m_dwReulst = XInputGetState(0, &m_state);

	// check if controller is connected
	if (m_dwReulst == ERROR_SUCCESS)
	{
		m_connected = true;
		std::cout << "XBOX CONTROLLER CONNECTED\n";
	}
	else
	{
		std::cout << "XBOX CONTROLLER NOT CONNECTED\n";
	}

	m_leftTrigger = 0.0f;
	m_rightTrigger = 0.0f;
}
XboxController::~XboxController()
{

}
void XboxController::update(float dt)
{
	m_buttonPressed = kNone;

	// reset
	m_leftTrigger = 0.0f;
	m_rightTrigger = 0.0f;

	// make sure there is a controller connected
	if (m_connected)
	{
		// get the state
		XInputGetState(0, &m_state);

		// gets the X and Y values and normalize results
		float LX = m_state.Gamepad.sThumbLX / 32767.0f; ///< left stick
		float LY = m_state.Gamepad.sThumbLY / 32767.0f; ///< left stick
		float RX = m_state.Gamepad.sThumbRX / 32767.0f; ///< right stick
		float RY = m_state.Gamepad.sThumbRY / 32767.0f; ///< right stick
		float LTrigger = m_state.Gamepad.bLeftTrigger / 255.0f; ///< left trigger
		float RTrigger = m_state.Gamepad.bRightTrigger / 255.0f; ///< right trigger

		// triggers
		if (m_state.Gamepad.bLeftTrigger)
		{
			m_leftTrigger = LTrigger;
			std::cout << "bLeftTrigger" << std::endl;
		}
		if (m_state.Gamepad.bRightTrigger)
		{
			m_rightTrigger = RTrigger;
			std::cout << "bRightTrigger" << std::endl;
		}

		// A/B/X/Y buttons
		if (m_state.Gamepad.wButtons & XINPUT_GAMEPAD_A)
		{
			m_buttonPressed = kA;
			std::cout << "XINPUT_GAMEPAD_A" << std::endl;
		}
		if (m_state.Gamepad.wButtons & XINPUT_GAMEPAD_B)
		{
			m_buttonPressed = kB;
			std::cout << "XINPUT_GAMEPAD_B" << std::endl;
		}
		if (m_state.Gamepad.wButtons & XINPUT_GAMEPAD_X)
		{
			m_buttonPressed = kX;
			std::cout << "XINPUT_GAMEPAD_X" << std::endl;
		}
		if (m_state.Gamepad.wButtons & XINPUT_GAMEPAD_Y)
		{
			m_buttonPressed = kY;
			std::cout << "XINPUT_GAMEPAD_Y" << std::endl;
		}

		// shoulder buttons
		if (m_state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
		{
			m_buttonPressed = kLS;
			std::cout << "XINPUT_GAMEPAD_LEFT_SHOULDER" << std::endl;
		}
		if (m_state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
		{
			m_buttonPressed = kRS;
			std::cout << "XINPUT_GAMEPAD_RIGHT_SHOULDER" << std::endl;
		}

		// deadzone and the sticks values
		float deadzone = 0.20f;

		if (fabs(LX) > deadzone)
		{
			m_leftStick.x = LX;
		}
		else
		{
			m_leftStick.x = 0.0f;
		}
		if (fabs(LY) > deadzone)
		{
			m_leftStick.y = LY;
		}
		else
		{
			m_leftStick.y = 0.0f;
		}

		if (fabs(RX) > deadzone)
		{
			m_rightStick.x = RX;
		}
		else
		{
			m_rightStick.x = 0.0f;
		}
		if (fabs(RY) > deadzone)
		{
			m_rightStick.y = RY;
		}
		else
		{
			m_rightStick.y = 0.0f;
		}

		// only display in debug mode, otherwise console is spammed
		#ifdef _DEBUG
			if (m_leftStick.x > 0 || m_leftStick.y > 0)
				std::cout << "L Stick: " << m_leftStick.x << " " << m_leftStick.y << std::endl;
			if (m_rightStick.x > 0 || m_rightStick.y > 0)
				std::cout << "R Stick: " << m_rightStick.x << " " << m_rightStick.y << std::endl;
		#endif
	}
}
XboxButtons XboxController::getLastButtonPressed()
{
	return m_buttonPressed;
}
