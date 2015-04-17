// Author  : Lewis Ward
// Program : Animation System Tech Demo
// Date    : 09/03/2015
#include "Camera.h"

Camera::Camera(float windowW, float windowH)
{
	m_hAngle = 0.0f;
	m_vAngle = 0.0f;
	m_distance = -75.0f;

	m_position = glm::vec3(0.0f, 0.0f, m_distance);
	m_dirX = glm::vec3(1.0f, 0.0f, 0.0f);
	m_dirY = glm::vec3(0.0f, 1.0f, 0.0f);

	m_rotX = 0.0f;
	m_rotY = 0.0f;
	m_lastX = windowW / 2;
	m_lastY = windowH / 2;

	// based upon code found here: http://glm.g-truc.net/0.9.4/code.html
	m_projection = glm::perspective(45.0f, windowW / windowH, 0.1f, 100.0f);

	//glm::mat4 ViewTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.0f));
	//glm::mat4 ViewRotateX = glm::rotate(ViewTranslate, 0.0f, glm::vec3(-1.0f, 0.0f, 0.0f));
	//m_view = glm::rotate(ViewRotateX, 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	//m_model = glm::scale(glm::mat4(1.0f), glm::vec3(0.75f));
}
Camera::~Camera()
{

}
void Camera::update(float dt, Event& events)
{
	// get lastest event 
	int eventCode = events.update();
	glm::vec2 mouse = events.mouseUpdate();

	int xDiff = mouse.x - m_lastX;
	int yDiff = mouse.y - m_lastY;
	m_lastX = mouse.x;
	m_lastY = mouse.y;

	// middle mouse button clicked/held
	if (eventCode == 3)
		zoomMove(dt, yDiff);

	// if left Crtl is pressed
	if (eventCode == 5)
	{
		verticalMove(dt, yDiff);
		horizontalMove(dt, xDiff);
	}

	//std::cout << m_distance << " " << xDiff << " " << yDiff << std::endl;

	// apply zoom
	m_position.z = m_distance;
	m_view = glm::translate(glm::mat4(1.0f), m_position);

	// apply rotation
	m_view = glm::rotate(m_view, glm::radians(m_vAngle), m_dirX); // vertical
	m_view = glm::rotate(m_view, glm::radians(m_hAngle), m_dirY); // horizontal

	// look at:  http://learnopengl.com/#!Getting-started/Camera
}
void Camera::update(float dt, Event& events, XboxController& controller)
{
	// get lastest event 
	int eventCode = events.update();
	glm::vec2 mouse = events.mouseUpdate();
	glm::vec2 lStick = controller.getLeftStick();
	glm::vec2 rStick = controller.getRightStick();

	int xDiff = mouse.x - m_lastX;
	int yDiff = mouse.y - m_lastY;
	m_lastX = mouse.x;
	m_lastY = mouse.y;

	// middle mouse button clicked/held
	if (eventCode == 3)
		zoomMove(dt, yDiff);

	// if left Crtl is pressed
	if (eventCode == 5)
	{
		verticalMove(dt, yDiff);
		horizontalMove(dt, xDiff);
	}

	///// ------------------------- Mouse & Keybaord -------------------------/////
	// apply zoom
	m_position.z = m_distance;
	m_view = glm::translate(glm::mat4(1.0f), m_position);

	// apply rotation
	m_view = glm::rotate(m_view, glm::radians(m_vAngle), m_dirX); // vertical
	m_view = glm::rotate(m_view, glm::radians(m_hAngle), m_dirY); // horizontal

	///// ------------------------- Xbox Controller -------------------------/////
	m_position.x += rStick.x;
	m_position.y += rStick.y;
	m_position.z = m_distance; // apply zoom
	m_view = glm::translate(glm::mat4(1.0f), m_position);

	// apply rotation
	m_vAngle += lStick.y;
	m_hAngle += lStick.x;
	m_view = glm::rotate(m_view, glm::radians(m_vAngle), m_dirX); // vertical
	m_view = glm::rotate(m_view, glm::radians(m_hAngle), m_dirY); // horizontal
}
void Camera::update(float dt, Event& events, XboxController& controller, glm::vec3& trajectoryPosition)
{
	// get lastest event 
	int eventCode = events.update();
	glm::vec2 mouse = events.mouseUpdate();
	glm::vec2 lStick = controller.getLeftStick();
	glm::vec2 rStick = controller.getRightStick();

	int xDiff = mouse.x - m_lastX;
	int yDiff = mouse.y - m_lastY;
	m_lastX = mouse.x;
	m_lastY = mouse.y;

	// middle mouse button clicked/held
	if (eventCode == 3)
		zoomMove(dt, yDiff);

	// if left Crtl is pressed
	if (eventCode == 5)
	{
		verticalMove(dt, yDiff);
		horizontalMove(dt, xDiff);
	}

	///// ------------------------- Mouse & Keybaord -------------------------/////
	// apply zoom
	m_position.z = trajectoryPosition.z + m_distance;
	m_view = glm::translate(glm::mat4(1.0f), m_position);

	// apply rotation
	m_view = glm::rotate(m_view, glm::radians(m_vAngle), m_dirX); // vertical
	m_view = glm::rotate(m_view, glm::radians(m_hAngle), m_dirY); // horizontal

	///// ------------------------- Xbox Controller -------------------------/////
	m_position.x = trajectoryPosition.x;
	m_position.y = trajectoryPosition.y;
	m_position.z = trajectoryPosition.z + m_distance; // apply zoom

	// compute zoom
	if (controller.getLeftTrigger())
		m_distance += controller.getLeftTrigger();
	else if (controller.getRightTrigger())
		m_distance -= controller.getRightTrigger();

	m_view = glm::translate(glm::mat4(1.0f), m_position);

	// apply rotation
	m_vAngle += lStick.y;
	m_hAngle += lStick.x;
	m_view = glm::rotate(m_view, glm::radians(m_vAngle), m_dirX); // vertical
	m_view = glm::rotate(m_view, glm::radians(m_hAngle), m_dirY); // horizontal
}
void Camera::verticalMove(float dt, int difference)
{
	if (difference > 0.0f)
		m_position.y -= 1.5f * dt;

	if (difference < 0.0f)
		m_position.y += 1.5f * dt;
}
void Camera::horizontalMove(float dt, int difference)
{
	if (difference > 0.0f)
		m_position.x += 1.5f * dt;

	if (difference < 0.0f)
		m_position.x -= 1.5f * dt;

}
void Camera::zoomMove(float dt, int difference)
{
	if (difference > 1.0f)
		m_distance += 1.5f * dt;

	if (difference < -1.0f)
		m_distance -= 1.5f * dt;
}
