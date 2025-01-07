#include "GeneralCamera.h"

#include "CameraObject.h"
#include "CTransform.h"
#include "GeneralCameraCInput.h"
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "UknittySettings.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <stdexcept>

namespace Uknitty
{

void GeneralCamera::OnAwake()
{
	m_projection = glm::mat4(1);
	m_view = glm::mat4(1);

	GameObject::AddCInput(new GeneralCameraCInput(this));
}

void GeneralCamera::OnLateUpdate([[maybe_unused]] float deltaTime)
{
	m_noFollowTransformSoNoControl = !m_followTransform;

	if(m_noFollowTransformSoNoControl)
	{
		CTransform* parentWorldTransform = GameObject::GetParent()->GetWorldTransform();
		m_pos = *parentWorldTransform->GetPosition();
		m_front = parentWorldTransform->GetForward();
		m_view = glm::lookAt(m_pos, m_pos + m_front, UP);
		m_projection = glm::perspective(glm::radians(m_fov), ASPECT_RATIO, NEAR_PLANE, FAR_PLANE);
	}
	else
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

		//REF: the part where I'm calculating the local rotation based on the view matrix,
		//is written with the help of claude.ai
		GameObject::GetLocalTransform()->SetPosition(m_pos);
		glm::mat4 rotationMatrix = glm::mat4(glm::mat3(m_view)); // Extract rotation part
		glm::vec3 rotation = glm::degrees(glm::eulerAngles(glm::quat_cast(rotationMatrix)));
		GameObject::GetLocalTransform()->SetRotation(-rotation);
	}
}

} // namespace Uknitty
