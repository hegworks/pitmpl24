#include "GeneralCamera.h"
#include "Transform.h"
#include <iostream>
#include <stdexcept>

GeneralCamera::GeneralCamera()
{
	m_projection = glm::mat4(1);
	m_view = glm::mat4(1);
}

GeneralCamera::~GeneralCamera()
{
}

void GeneralCamera::ProcessMousePosition(double xPos, double yPos)
{
	switch(m_cameraType)
	{
		case GeneralCamera::CameraType::TOP_DOWN_FOLLOW:
			break;
		case GeneralCamera::CameraType::THIRD_PERSON_FOLLOW:
		case GeneralCamera::CameraType::FIRST_PERSON:
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

			switch(m_cameraType)
			{
				case GeneralCamera::CameraType::THIRD_PERSON_FOLLOW:
					if(m_pitch > PITCH_MAX_THIRD_PERSON) m_pitch = PITCH_MAX_THIRD_PERSON;
					if(m_pitch < PITCH_MIN_THIRD_PERSON) m_pitch = PITCH_MIN_THIRD_PERSON;
					break;
				case GeneralCamera::CameraType::FIRST_PERSON:
					if(m_pitch > PITCH_MAX_DEFAULT) m_pitch = PITCH_MAX_DEFAULT;
					if(m_pitch < PITCH_MIN_DEFAULT) m_pitch = PITCH_MIN_DEFAULT;
					break;
				case GeneralCamera::CameraType::TOP_DOWN_FOLLOW:
				default:
					throw std::runtime_error("unknown camera type");
			}
		}
		break;
		default:
			throw std::runtime_error("unknown camera type");
	}
}

void GeneralCamera::ProcessKeyboard(IKeyboard* iKeyboard)
{
}

void GeneralCamera::KeyDown(Key key)
{
	if(key == TYPE_SWITCH_KEY)
	{
		int nextCameraType = ((int)m_cameraType + 1) % TOTAL_CAMERA_TYPES;
		m_cameraType = static_cast<CameraType>(nextCameraType);
	}
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
			m_pos = *m_followTransform->GetPosition() + FOLLOW_OFFSET_TOPDOWN;

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
		{
			// Calculate the camera's offset position relative to the followTransform
			glm::vec3 followPos = *m_followTransform->GetPosition();
			followPos.y += FOLLOW_OFFSET_Y_THIRD_PERSON;

			// Convert pitch and yaw to a direction vector for the offset
			glm::vec3 offsetDir;
            offsetDir.x = static_cast<float>(cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch)));
            offsetDir.y = static_cast<float>(sin(glm::radians(m_pitch)));
            offsetDir.z = static_cast<float>(sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch)));
			offsetDir = glm::normalize(offsetDir);

			// Calculate the camera position
			m_pos = followPos - offsetDir * FOLLOW_DISTANCE_THIRD_PERSON;

			// Calculate the front vector to look at the followTransform
			m_front = glm::normalize(followPos - m_pos);

			// Update the view matrix to look at the followTransform
			m_view = glm::lookAt(m_pos, followPos, UP);

			// Update the projection matrix
			m_projection = glm::perspective(glm::radians(m_fov), ASPECT_RATIO, NEAR_PLANE, FAR_PLANE);
		}
		break;
		case GeneralCamera::CameraType::FIRST_PERSON:
		{
#pragma region simple version (should hide the model)
			// Position the camera at the followTransform's position
			glm::vec3 followPos = *m_followTransform->GetPosition();
			followPos.y += FOLLOW_OFFSET_Y_FIRST_PERSON; //TODO remove FOLLOW_OFFSET_Y_FIRST_PERSON and apply it directly to followTransform
			m_pos = followPos;

			// Calculate the front vector based on yaw and pitch
			glm::vec3 direction;
            direction.x = static_cast<float>(cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch)));
            direction.y = static_cast<float>(sin(glm::radians(m_pitch)));
            direction.z = static_cast<float>(sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch)));
			m_front = glm::normalize(direction);

			// Update the view matrix to look in the front direction
			m_view = glm::lookAt(m_pos, m_pos + m_front, UP);

			// Update the projection matrix
			m_projection = glm::perspective(glm::radians(m_fov), ASPECT_RATIO, NEAR_PLANE, FAR_PLANE);
#pragma endregion simple version (should hide the model)

#pragma region complex & weird version with offset
			/*
			// Step 1: Position the camera at the followTransform's position with an offset
			glm::vec3 followTransformPos = *m_followTransform->GetPosition();
			glm::vec3 offset = FOLLOW_OFFSET_FIRST_PERSON; // Use m_followOffset to specify a local offset
			glm::vec3 up = UP; // Define your world "up" vector (e.g., glm::vec3(0.0f, 1.0f, 0.0f))

			// Apply the offset (e.g., slightly above the followTransform's position)
			m_pos = followTransformPos + offset.x * glm::normalize(glm::cross(m_front, up)) // Horizontal offset
				+ offset.y * up                                    // Vertical offset
				+ offset.z * glm::normalize(m_front);              // Forward/backward offset

			// Step 2: Calculate the front vector based on yaw and pitch
			glm::vec3 direction;
			direction.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
			direction.y = sin(glm::radians(m_pitch));
			direction.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
			m_front = glm::normalize(direction);

			// Step 3: Update the view matrix to look in the front direction
			m_view = glm::lookAt(m_pos, m_pos + m_front, up);

			// Step 4: Update the projection matrix
			m_projection = glm::perspective(glm::radians(m_fov), ASPECT_RATIO, NEAR_PLANE, FAR_PLANE);
			*/
#pragma endregion complex & not working version with offset
		}
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

void GeneralCamera::Destroy()
{
	delete this;
}
