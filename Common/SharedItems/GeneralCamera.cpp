#include "GeneralCamera.h"

#include "CameraObject.h"
#include "CTransform.h"
#include "GeneralCameraCInput.h"
#include "UknittySettings.h"
#include <iostream>
#include <stdexcept>

namespace Uknitty
{

void GeneralCamera::OnLateUpdate(float deltaTime)
{
	if(!m_followTransform)
	{
		std::cerr << "WARNING::GeneralCamera follow transform is nullptr" << std::endl;
		return;
	}

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
		}
		break;
		default:
			throw std::runtime_error("Unknown camera type");
	}

	CameraObject::UpdateLocalTransformMatrix(m_projection * m_view);
}

void GeneralCamera::OnAwake()
{
	m_projection = glm::mat4(1);
	m_view = glm::mat4(1);

	GameObject::AddCInput(new GeneralCameraCInput(this));

	std::cout << "general camera created" << std::endl;
}

void GeneralCamera::OnDestroy()
{
	std::cout << "Destroying GeneralCamera" << std::endl;
	delete this;
}

} // namespace Uknitty
