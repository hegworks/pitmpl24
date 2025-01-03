#pragma once

#include "CameraObject.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Uknitty
{

class GeneralCamera : public Uknitty::CameraObject
{
public:
	// Inherited via CameraObject
	virtual glm::mat4 GetView() override { return m_view; }
	virtual glm::mat4 GetProjection() override { return m_projection; }
	virtual glm::vec3 GetForward() override { return m_front; }
	virtual glm::vec3 GetRight() override { return glm::normalize(glm::cross(m_front, UP)); };
	virtual float GetFov() const override { return m_fov; }

	virtual void OnAwake() override;
	virtual void OnLateUpdate(float deltaTime) override;
	virtual void OnDestroy() override;

	virtual void SetFollowTransform(Uknitty::CTransform* followTransform) override { m_followTransform = followTransform; }

	const float PITCH_MAX_THIRD_PERSON = 10.0f;
	const float PITCH_MIN_THIRD_PERSON = -45.0f;
	const float PITCH_MAX_FIRST_PERSON = 80.0f;
	const float PITCH_MIN_FIRST_PERSON = -80.0f;

	const int TOTAL_CAMERA_TYPES = 3;
	enum class CameraType
	{
		TOP_DOWN_FOLLOW,
		THIRD_PERSON_FOLLOW,
		FIRST_PERSON,
	};
	CameraType GetCameraType() const { return m_cameraType; }

private:
	// settings
	const float YAW_DEFAULT = -90.0f;
	const float FOV_DEFAULT = 45.0f;
	const glm::vec3 POS_INITIAL = glm::vec3(0);
	const glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0f);
	const glm::vec3 FRONT_INITIAL = glm::vec3(0.0f, 1.0f, 0.0f);
	const float PITCH_MAX_DEFAULT = 89.0f;
	const float PITCH_MIN_DEFAULT = -89.0f;

	const float NEAR_PLANE = 0.1f;
	const float FAR_PLANE = 1000.0f;

	//const glm::vec3 FOLLOW_OFFSET_TOPDOWN = glm::vec3(0, 12, -8);
	const glm::vec3 FOLLOW_OFFSET_TOPDOWN = glm::vec3(0, 35, -4);

	const float FOLLOW_OFFSET_Y_THIRD_PERSON = 2.0f;
	const float FOLLOW_DISTANCE_THIRD_PERSON = 5.0f;

	const glm::vec3 FOLLOW_OFFSET_FIRST_PERSON = glm::vec3(0, 2, 0.2);
	const float FOLLOW_OFFSET_Y_FIRST_PERSON = 2.0f;
	const float FOLLOW_DISTANCE_FIRST_PERSON = 0.5f;

	// variables
	Uknitty::CTransform* m_followTransform = nullptr;
	CameraType m_cameraType = CameraType::TOP_DOWN_FOLLOW;

	double m_yaw = YAW_DEFAULT;
	double m_pitch = 0;
	float m_fov = FOV_DEFAULT;

	glm::mat4 m_view;
	glm::mat4 m_projection;

	glm::vec3 m_pos = POS_INITIAL;
	glm::vec3 m_front = FRONT_INITIAL;

	bool m_isForwardKeyDown = false;
	bool m_isBackwardKeyDown = false;
	bool m_isLeftKeyDown = false;
	bool m_isRightKeyDown = false;

#pragma region Used in GeneralCameraCInput
public:
	double GetYaw() const { return m_yaw; }
	double GetPitch() const { return m_pitch; }
	void SetYaw(double yaw) { m_yaw = yaw; }
	void SetPitch(double pitch) { m_pitch = pitch; }
	void SetCameraType(CameraType cameraType) { m_cameraType = cameraType; }
#pragma endregion

};

} // namespace Uknitty
