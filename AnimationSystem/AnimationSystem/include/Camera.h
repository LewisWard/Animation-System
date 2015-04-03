// Author  : Lewis Ward
// Program : Animation System Tech Demo
// Date    : 09/03/2015
#pragma once
#include "Window.h"
#include "Events.h"

//----------------------------------------------------------------------------------------------------------------------
/// \brief Camera to move around the scene
//----------------------------------------------------------------------------------------------------------------------
class Camera
{
public:
	/// \brief Constructor
	Camera(float windowW, float windowH);
	/// \brief Destructor
	~Camera();

	/// \brief update the camera
	/// \prama float delta time
	/// \prama float delta time
	void update(float dt, Event& events);

	/// \brief get the model matrix
	/// \prama glm::mat4 model matrix
	inline glm::mat4& modelMatrix()
	{ return m_model; }

	/// \brief get the view matrix
	/// \prama glm::mat4 view matrix
	inline glm::mat4& viewMatrix()
	{ return m_view; }

	/// \brief get the projection matrix
	/// \prama glm::mat4 projection matrix
	inline glm::mat4& projectionMatrix()
	{ return m_projection; }

	/// \brief get the Model-View-Projection matrix
	/// \prama glm::mat4 Model-View-Projection matrix
	inline glm::mat4& MVPMatrix()
	{ return m_projection * m_view; }

private:

	void verticalMove(float dt, int difference);
	void horizontalMove(float dt, int difference);
	void zoomMove(float dt, int difference);

	float m_hAngle; ///< the horizontal angle of the camera 
	float m_vAngle; ///< the vertical angle of the camera 
	float m_distance; ///< the distance from the target/origin

	int m_lastX;
	int m_lastY;
	float m_rotX;
	float m_rotY;

	glm::vec3 m_position; ///< camera position
	glm::vec3 m_dirX; ///< for working out the 'up' direction of the camera
	glm::vec3 m_dirY; ///< for working out the 'up' direction of the camera

	glm::mat4 m_model; ///< view matrix
	glm::mat4 m_view; ///< view matrix
	glm::mat4 m_projection; ///< projection matrix
};