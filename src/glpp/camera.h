#ifndef GLPP_CAMERA_H
#define GLPP_CAMERA_H

#include <GL/glew.h>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace glpp
{

enum class Direction {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

namespace
{

auto constexpr DEFAULT_YAW   = -90.0f;
auto constexpr DEFAULT_PITCH = 0.0f;

auto constexpr DEFAULT_MOUSE_SPEED = 3.0f;
auto constexpr DEFAULT_MOUSE_SENSITIVITY = 0.25f;

auto constexpr DEFAULT_ZOOM = 45.0f;

}	// namespace

class Camera
{
public:
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
		   glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
		   GLfloat yaw = DEFAULT_YAW, GLfloat pitch = DEFAULT_PITCH);

	glm::mat4 getView() const;
	glm::vec3 getPosition() const;
	GLfloat getZoom() const;

	void move(Direction direction, GLfloat timeDelta);
	void turn(GLfloat xoff, GLfloat yoff, GLboolean constrainPitch = true);
	void zoom(GLfloat yoff);

private:
	void updateVectors();

private:
	glm::vec3 m_pos;
	glm::vec3 m_front = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 m_up;
	glm::vec3 m_right;
	glm::vec3 m_worldUp;

	GLfloat m_yaw = DEFAULT_YAW;
	GLfloat m_pitch = DEFAULT_PITCH;

	GLfloat m_speed = DEFAULT_MOUSE_SPEED;
	GLfloat m_sensitivity = DEFAULT_MOUSE_SENSITIVITY;

	GLfloat m_zoom = DEFAULT_ZOOM;
};

}

#endif // GLPP_CAMERA_H
