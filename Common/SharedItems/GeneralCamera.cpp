#include "GeneralCamera.h"
#include "Transform.h"
#include <stdexcept>

GeneralCamera::GeneralCamera()
{
	m_projection = glm::mat4(1);
	m_view = glm::mat4(1);
}

GeneralCamera::~GeneralCamera()
{
}

void GeneralCamera::MouseCallback(double xPos, double yPos)
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

void GeneralCamera::ProcessInput(IKeyboard* iKeyboard)
{
}

void GeneralCamera::KeyDown(Key key)
{
}

void GeneralCamera::KeyUp(Key key)
{
}

void GeneralCamera::Update(float deltaTime)
{
}

void GeneralCamera::LateUpdate(float deltaTime)
{
	switch(m_cameraType)
	{
		case GeneralCamera::CameraType::TOP_DOWN_FOLLOW:
		{
			// Update the camera position to maintain the offset from the follow transform
			m_pos = *m_followTransform->GetPosition() + m_followOffset;

			// Calculate the direction vector to look at the follow transform
			glm::vec3 direction = glm::normalize(*m_followTransform->GetPosition() - m_pos);

			// Update the camera's front vector to look at the follow transform
			m_front = direction;

			// Update the view matrix to look at the follow transform
			m_view = glm::lookAt(m_pos, *m_followTransform->GetPosition(), UP);

			// Update the projection matrix
			m_projection = glm::perspective(glm::radians(m_fov), ASPECT_RATIO, NEAR_PLANE, FAR_PLANE);
		}
		break;
		case GeneralCamera::CameraType::THIRD_PERSON_FOLLOW:
			break;
		case GeneralCamera::CameraType::FIRST_PERSON:
			break;
		default:
			throw std::runtime_error("Unknown camera type");
	}

}

void GeneralCamera::GetCameraProperties(glm::vec3& pos, glm::vec3& front, glm::vec3& up) const
{
	pos = m_pos;
	front = m_front;
	up = UP;
}
