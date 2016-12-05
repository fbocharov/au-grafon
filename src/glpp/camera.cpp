#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace glpp
{

Camera::Camera(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch)
	: m_pos(position)
	, m_worldUp(up)
	, m_yaw(yaw)
	, m_pitch(pitch)
{
	updateVectors();
}

glm::mat4 Camera::getView() const
{
	return glm::lookAt(m_pos, m_pos + m_front, m_up);
}

glm::vec3 Camera::getPosition() const
{
	return m_pos;
}

GLfloat Camera::getZoom() const
{
	return m_zoom;
}

void Camera::move(Direction direction, GLfloat timeDelta)
{
	GLfloat velocity = m_speed * timeDelta;
	switch (direction) {
		case Direction::FORWARD:
			m_pos += m_front * velocity;
			break;
		case Direction::BACKWARD:
			m_pos -= m_front * velocity;
			break;
		case Direction::LEFT:
			m_pos -= m_right * velocity;
			break;
		case Direction::RIGHT:
			m_pos += m_right * velocity;
			break;
	}
}

void Camera::turn(GLfloat xoff, GLfloat yoff, GLboolean constrainPitch)
{
	xoff *= m_sensitivity;
	yoff *= m_sensitivity;

	m_yaw   += xoff;
	m_pitch += yoff;

	if (constrainPitch) {
		if (m_pitch > 89.0f)  m_pitch = 89.0f;
		if (m_pitch < -89.0f) m_pitch = -89.0f;
	}

	updateVectors();
}

void Camera::zoom(GLfloat yoff)
{
	if (m_zoom <= 1.0f) m_zoom = 1.0f;
	if (m_zoom >= 45.0f) m_zoom = 45.0f;
	if (m_zoom >= 1.0f && m_zoom <= 45.0f) m_zoom -= yoff;
}


void Camera::updateVectors()
{
	glm::vec3 front;
	front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	front.y = sin(glm::radians(m_pitch));
	front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	m_front = glm::normalize(front);

	m_right = glm::normalize(glm::cross(m_front, m_worldUp));
	m_up = glm::normalize(glm::cross(m_right, m_front));
}


}	// namespace glpp
