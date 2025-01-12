#include "GeneralCamera.h"

#include "btBulletDynamicsCommon.h"
#include "CameraObject.h"
#include "CPhysics.h"
#include "CTransform.h"
#include "GeneralCameraCInput.h"
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "UknittySettings.h"
#include <Engine.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <PhysicsCollisionFilters.h>
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
	switch(m_mode)
	{
		case Mode::FOLLOW:
			FollowCamera(deltaTime);
			break;
		case Mode::CHILD_OF_STH:
			ChildOfSthCamera();
			break;
		default:
			throw std::runtime_error("Unknown camera mode");
	}
}

void GeneralCamera::FollowCamera(float deltaTime)
{
	if(m_mode != Mode::FOLLOW)
		return;

	if(!m_followTransform)
	{
		throw std::runtime_error("GeneralCamera mode is set to FOLLOW but it has no follow transform");
	}

	switch(m_followType)
	{
		case GeneralCamera::FollowType::TOP_DOWN_FOLLOW:
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
		case GeneralCamera::FollowType::THIRD_PERSON_FOLLOW:
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
			glm::vec3 unclippedPos = followPos - offsetDir * FOLLOW_DISTANCE_THIRD_PERSON;

			{ // camera clipping
				glm::vec3 fromVec = *m_followTransform->GetPosition() + glm::vec3(0, 1.6, 0);
				glm::vec3 toVec = unclippedPos;
				const btVector3 from = Uknitty::CPhysics::GLMVec3ToBtVec3(fromVec);
				const btVector3 to = Uknitty::CPhysics::GLMVec3ToBtVec3(toVec);
				btCollisionWorld::ClosestRayResultCallback  closestResults = btCollisionWorld::ClosestRayResultCallback(from, to);
				closestResults.m_collisionFilterGroup = COLL_GROUP_CAMERA_CLIP;
				closestResults.m_collisionFilterMask = COLL_MASK_CAMERA_CLIP;
				Uknitty::Engine::GetInstance()->GetDynamicsWorld()->rayTest(from, to, closestResults);
				if(closestResults.hasHit())
				{
					glm::vec3 destination = Uknitty::CPhysics::BtVec3ToGLMVec3(closestResults.m_hitPointWorld + closestResults.m_hitNormalWorld * 0.2f);
					m_pos = glm::mix(m_pos, destination, PUSH_IN_SPEED * deltaTime);
					m_clippingHadHit = true;
				}
				else
				{
					if(m_clippingHadHit) // was pushed in, so pushing out
					{
						glm::vec3 destination = unclippedPos;
						m_pos = glm::mix(m_pos, destination, PUSH_OUT_SPEED * deltaTime);
						if(glm::distance(m_pos, destination) < PUSH_OUT_THRESHOLD)
						{
							m_clippingHadHit = false;
						}
					}
					else // was never pushed in, or pushing out is finished
					{
						m_pos = unclippedPos;
					}
				}
			}

			// Calculate the front vector to look at the followTransform
			m_front = glm::normalize(followPos - m_pos);

			// Update the view matrix to look at the followTransform
			m_view = glm::lookAt(m_pos, followPos, UP);

			// Update the projection matrix
			m_projection = glm::perspective(glm::radians(m_fov), ASPECT_RATIO, NEAR_PLANE, FAR_PLANE);
		}
		break;
		case GeneralCamera::FollowType::FIRST_PERSON:
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

void GeneralCamera::ChildOfSthCamera()
{
	if(m_mode != Mode::CHILD_OF_STH)
		return;

	if(!GameObject::GetParent())
	{
		throw std::runtime_error("GeneralCamera mode is set to CHILD_OF_STH but it has no parent");
	}

	CTransform* parentWorldTransform = GameObject::GetParent()->GetWorldTransform();
	m_pos = *parentWorldTransform->GetPosition();
	m_front = parentWorldTransform->GetForward();
	m_view = glm::lookAt(m_pos, m_pos + m_front, UP);
	m_projection = glm::perspective(glm::radians(m_fov), ASPECT_RATIO, NEAR_PLANE, FAR_PLANE);
}

} // namespace Uknitty
