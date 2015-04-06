#pragma once
#include "Window.h"
#include <Windows.h>
#include <Xinput.h>

class XboxController
{
public:

	XboxController();
	~XboxController();

	void update(float dt);

	inline bool isConnectedState()
	{ return m_connected; }

	inline glm::vec2 getLeftStick()
	{ return m_leftStick; }

	inline glm::vec2 getRightStick()
	{ return m_rightStick; }

private:

	DWORD m_dwReulst;
	XINPUT_STATE m_state;
	bool m_connected;
	glm::vec2 m_leftStick;
	glm::vec2 m_rightStick;
};
