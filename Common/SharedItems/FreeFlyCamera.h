#pragma once

#include "Camera.h"
#include "Common.h"
#include "IInput.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class FreeFlyCamera : public Camera
{
public:
	FreeFlyCamera();
	~FreeFlyCamera();

	virtual void MouseCallback(double xPos, double yPos);
	virtual void ProcessInput(IKeyboard* iKeyboard);
	virtual void Update(float deltaTime);
	virtual void KeyDown(Key key) {};
	virtual void KeyUp(Key key) {};

	virtual void GetCameraProperties(glm::vec3& pos, glm::vec3& front, glm::vec3& up) const;
	virtual float GetFov() const { return m_fov; }

private:
	// settings
	const float YAW_DEFAULT = -90.0f;
	const float FOV_DEFAULT = 45.0f;
	const float SPEED_DEFAULT = 10.0f;
	const float SPEED_BOOSTED = 20.0f;
	const glm::vec3 POS_INITIAL = glm::vec3(0.0f, 2.0f, 0.0f);
	const glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0f);
	const glm::vec3 FRONT_INITIAL = glm::vec3(0.0f, 1.0f, 0.0f);
	const float PITCH_MAX = 89.0f;
	const float NEAR_PLANE = 0.1f;
	const float FAR_PLANE = 1000.0f;
	const float MOUSE_SENSITIVITY = 0.05f;

	const Key UP_KEY = Key::W;
	const Key DOWN_KEY = Key::S;
	const Key RIGHT_KEY = Key::D;
	const Key LEFT_KEY = Key::A;
	const Key ASCEND_KEY = Key::Q;
	const Key DESCEND_KEY = Key::E;
	const Key SPEED_BOOST_KEY = Key::SHIFT_LEFT;

	// variables
	double m_yaw = YAW_DEFAULT;
	double m_pitch = 0;
	float m_fov = FOV_DEFAULT;

	glm::mat4 m_view;
	glm::mat4 m_projection;

	bool m_isFirstMouse = true;
	double m_lastX = SCRWIDTH / 2.0;
	double m_lastY = SCRHEIGHT / 2.0;

	glm::vec3 m_pos = POS_INITIAL;
	glm::vec3 m_front = FRONT_INITIAL;

	bool m_isUpKeyPressed = false;
	bool m_isDownKeyPressed = false;
	bool m_isLeftKeyPressed = false;
	bool m_isRightKeyPressed = false;
	bool m_isBoostKeyPressed = false;
	bool m_isAscendKeyPressed = false;
	bool m_isDescendKeyPressed = false;

	// Inherited via Camera
	glm::mat4 GetView() override;
	glm::mat4 GetProjection() override;
};

inline FreeFlyCamera::FreeFlyCamera()
{
}

inline FreeFlyCamera::~FreeFlyCamera()
{
}

inline void FreeFlyCamera::MouseCallback(double xPos, double yPos)
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

inline void FreeFlyCamera::ProcessInput(IKeyboard* iKeyboard)
{
	m_isUpKeyPressed = iKeyboard->GetKey(UP_KEY);
	m_isDownKeyPressed = iKeyboard->GetKey(DOWN_KEY);
	m_isLeftKeyPressed = iKeyboard->GetKey(LEFT_KEY);
	m_isRightKeyPressed = iKeyboard->GetKey(RIGHT_KEY);
	m_isAscendKeyPressed = iKeyboard->GetKey(ASCEND_KEY);
	m_isDescendKeyPressed = iKeyboard->GetKey(DESCEND_KEY);
	m_isBoostKeyPressed = iKeyboard->GetKey(SPEED_BOOST_KEY);
}

inline void FreeFlyCamera::Update(float deltaTime)
{
	const float cameraSpeed = m_isBoostKeyPressed ? SPEED_BOOSTED : SPEED_DEFAULT;
	if(m_isUpKeyPressed)
		m_pos += cameraSpeed * m_front * deltaTime;
	if(m_isDownKeyPressed)
		m_pos -= cameraSpeed * m_front * deltaTime;
	if(m_isLeftKeyPressed)
		m_pos -= glm::normalize(glm::cross(m_front, UP)) * cameraSpeed * deltaTime;
	if(m_isRightKeyPressed)
		m_pos += glm::normalize(glm::cross(m_front, UP)) * cameraSpeed * deltaTime;
	if(m_isAscendKeyPressed)
		m_pos.y += cameraSpeed * deltaTime;
	if(m_isDescendKeyPressed)
		m_pos.y -= cameraSpeed * deltaTime;

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

inline void FreeFlyCamera::GetCameraProperties(glm::vec3& pos, glm::vec3& front, glm::vec3& up) const
{
	pos = m_pos;
	front = m_front;
	up = UP;
}

glm::mat4 FreeFlyCamera::GetView()
{
	return m_view;
}

glm::mat4 FreeFlyCamera::GetProjection()
{
	return m_projection;
}
