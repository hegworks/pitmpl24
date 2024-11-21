#pragma once

#include "Common.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
	Camera();
	~Camera();

	virtual void MouseCallback(double xPos, double yPos);
	virtual void Update(float deltaTime);

	virtual void GetCameraProperties(glm::vec3& pos, glm::vec3& front, glm::vec3& up) const;
	virtual float GetFov() const { return m_fov; }
	glm::mat4 GetView() { return m_view; }
	glm::mat4 GetProjection() { return m_projection; }

private:
	//------settings
	const float YAW_DEFAULT = -90.0f;
	const float FOV_DEFAULT = 45.0f;
	const float SPEED_DEFAULT = 10.0f;
	const float SPEED_BOOSTED = 20.0f;
	const glm::vec3 POS_INITIAL = glm::vec3(0.0, 5.0f, 10.0);
	const glm::vec3 UP = glm::vec3(0.0, 1.0, 0.0);
	const glm::vec3 FRONT_INITIAL = glm::vec3(0.0, 1.0, 0.0);
	const float PITCH_MAX = 89.0f;
	const float MOUSE_SENSITIVITY = 0.05f;
	const float NEAR_PLANE = 0.1f;
	const float FAR_PLANE = 100.0f;
	//=====settings


	//-----variables
	double m_yaw = YAW_DEFAULT;
	double m_pitch = 0;
	float m_fov = FOV_DEFAULT;

	bool m_isFirstMouse = true;
	double m_lastX = SCRWIDTH / 2.0;
	double m_lastY = SCRHEIGHT / 2.0;

	glm::vec3 m_pos = POS_INITIAL;
	glm::vec3 m_front = FRONT_INITIAL;
	glm::mat4 m_projection = glm::mat4(1.0);
	glm::mat4 m_view = glm::mat4(1.0);

	bool m_isUpKeyPressed = false;
	bool m_isDownKeyPressed = false;
	bool m_isLeftKeyPressed = false;
	bool m_isRightKeyPressed = false;
	bool m_isBoostKeyPressed = false;
	bool m_isAscendKeyPressed = false;
	bool m_isDescendKeyPressed = false;
	//======variables
};

inline Camera::Camera()
{
}

inline Camera::~Camera()
{
}

inline void Camera::MouseCallback(double xPos, double yPos)
{
	if(m_isFirstMouse) // initially set to true
	{
		m_lastX = xPos;
		m_lastY = yPos;
		m_isFirstMouse = false;
	}

	double xOffset = xPos - m_lastX;
	double yOffset = m_lastY - yPos; // reversed since y-coordinates range from bottom to top
	m_lastX = xPos;
	m_lastY = yPos;

	xOffset *= MOUSE_SENSITIVITY;
	yOffset *= MOUSE_SENSITIVITY;

	m_yaw += xOffset;
	m_pitch += yOffset;

	if(m_pitch > PITCH_MAX)
		m_pitch = PITCH_MAX;
	if(m_pitch < -PITCH_MAX)
		m_pitch = -PITCH_MAX;
}

inline void Camera::Update(float deltaTime)
{
	const float cameraSpeed = m_isBoostKeyPressed ? SPEED_BOOSTED : SPEED_DEFAULT;
	if(m_isUpKeyPressed)
		m_pos += cameraSpeed * glm::normalize(glm::vec3(m_front.x, 0, m_front.z)) * deltaTime;
	if(m_isDownKeyPressed)
		m_pos -= cameraSpeed * glm::normalize(glm::vec3(m_front.x, 0, m_front.z)) * deltaTime;
	if(m_isLeftKeyPressed)
		m_pos -= glm::normalize(glm::cross(m_front, UP)) * cameraSpeed * deltaTime;
	if(m_isRightKeyPressed)
		m_pos += glm::normalize(glm::cross(m_front, UP)) * cameraSpeed * deltaTime;


	glm::vec3 direction;
	const float yawF = static_cast<float>(m_yaw);
	const float pitchF = static_cast<float>(m_pitch);
	direction.x = cos(glm::radians(yawF)) * cos(glm::radians(pitchF));
	direction.y = sin(glm::radians(pitchF));
	direction.z = sin(glm::radians(yawF)) * cos(glm::radians(pitchF));
	m_front = glm::normalize(direction);


	m_view = glm::lookAt(m_pos, m_pos + m_front, UP);
	m_projection = glm::perspective(glm::radians(m_fov), ASPECT_RATIO, NEAR_PLANE, FAR_PLANE);
}

inline void Camera::GetCameraProperties(glm::vec3& pos, glm::vec3& front, glm::vec3& up) const
{
	pos = m_pos;
	front = m_front;
	up = UP;
}
