// Author  : Lewis Ward
// Program : Animation System Tech Demo
// Date    : 09/03/2015
#include "Camera.h"

Camera::Camera(float windowW, float windowH)
{
	m_hAngle = 180.0f;
	m_vAngle = 25.0f;
	m_distance = -75.0f;

	m_position = glm::vec3(0.0f, 50.0f, m_distance);
	m_dirX = glm::vec3(1.0f, 0.0f, 0.0f);
	m_dirY = glm::vec3(0.0f, 1.0f, 0.0f);

	// rotate camera
	m_view = glm::rotate(m_view, glm::radians(m_vAngle), m_dirX); // vertical
	m_view = glm::rotate(m_view, glm::radians(m_hAngle), m_dirY); // vertical

	// based upon code found here: http://glm.g-truc.net/0.9.4/code.html
	m_projection = glm::perspective(45.0f, windowW / windowH, 0.1f, 1000.0f);
}
Camera::~Camera()
{

}
void Camera::update(float dt, Event& events)
{
	// get lastest event 
	int eventCode = events.update();
	glm::vec2 mouse = events.mouseUpdate();

	// apply zoom
	m_position.z = m_distance;
	m_view = glm::translate(glm::mat4(1.0f), m_position);

	// apply rotation
	m_view = glm::rotate(m_view, glm::radians(m_vAngle), m_dirX); // vertical
	m_view = glm::rotate(m_view, glm::radians(m_hAngle), m_dirY); // horizontal
}
void Camera::update(float dt, Event& events, XboxController& controller, glm::mat4& trajectoryPosition)
{
	// get lastest event 
	int eventCode = events.update();
	glm::vec2 mouse = events.mouseUpdate();
	glm::vec2 lStick = controller.getLeftStick();
	glm::vec2 rStick = controller.getRightStick();

	///// ------------------------- Mouse & Keybaord -------------------------/////
	//// apply zoom
	//m_position.z = trajectoryPosition.z + m_distance;
	//m_view = glm::translate(glm::mat4(1.0f), m_position);
	//
	//// apply rotation
	//m_view = glm::rotate(m_view, glm::radians(m_vAngle), m_dirX); // vertical
	//m_view = glm::rotate(m_view, glm::radians(m_hAngle), m_dirY); // horizontal

	///// ------------------------- Xbox Controller -------------------------/////
	//m_position.x = trajectoryPosition.x;
	//m_position.y = trajectoryPosition.y - 50.0f;
	//m_position.z = fabs(trajectoryPosition.z) + m_distance; // apply zoom

	//m_position.x = trajectoryPosition.x;
	//m_position.y = trajectoryPosition.y;
	//m_position.z = trajectoryPosition.z + m_distance; // apply zoom

	// compute zoom
	if (controller.getLeftTrigger())
		m_distance += controller.getLeftTrigger();
	else if (controller.getRightTrigger())
		m_distance -= controller.getRightTrigger();

	// invert 
	//m_position.x = -m_position.x;
	
	std::cout << trajectoryPosition[3].x << " " << trajectoryPosition[3].y << " " << trajectoryPosition[3].z << std::endl;
	//std::cout << "tra :" << trajectory[3].x << " " << trajectory[3].y << " " << trajectory[3].z << std::endl;
	//std::cout << m_view[3].x << " " << m_view[3].y << " " << m_view[3].z << std::endl;
	
	// apply rotation
	m_vAngle += lStick.y;
	m_hAngle += -lStick.x;

	m_model[3].x = trajectoryPosition[3].x;
	m_model[3].y = trajectoryPosition[3].y + m_position.y;
	m_model[3].z = trajectoryPosition[3].z + m_distance; // apply zoom
	
	//m_view = glm::inverse(glm::rotate(m_model, glm::radians(m_vAngle), m_dirX)); // vertical
	m_view = glm::inverse(glm::rotate(m_model, glm::radians(m_hAngle), m_dirY)); // horizontal

	std::cout << "pos :" << m_hAngle << " " << m_vAngle << " " << m_distance << std::endl;
	//std::cout << "rot :" << rotated.x << " " << rotated.y << " " << rotated.z << std::endl;
	std::cout << "vie :" << m_view[3].x << " " << m_view[3].y << " " << m_view[3].z << std::endl;
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
