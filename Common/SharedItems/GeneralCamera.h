#pragma once

#include "ICamera.h"
#include <Common.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Uknitty
{
class Transform;
}

class GeneralCamera : public Uknitty::ICamera
{

public:
	GeneralCamera();
	~GeneralCamera();

	// Inherited via ICamera
	virtual glm::mat4 GetView() override { return m_view; }
	virtual glm::mat4 GetProjection() override { return m_projection; }
	virtual glm::vec3 GetForward() override { return m_front; }
	virtual glm::vec3 GetRight() override { return glm::normalize(glm::cross(m_front, UP)); };
	virtual void GetCameraProperties(glm::vec3& pos, glm::vec3& front, glm::vec3& up) const override;
	virtual float GetFov() const override { return m_fov; }

	// Inherited via IInputProcessor
	virtual void MouseCallback(double xPos, double yPos) override;
	virtual void ProcessInput(IKeyboard* iKeyboard) override;
	virtual void KeyDown(Key key) override;
	virtual void KeyUp(Key key) override;

	// Inherited via ILifeCycle
	virtual void Update(float deltaTime) override;
	virtual void LateUpdate(float deltaTime) override;

	const int TOTAL_CAMERA_TYPES = 3;
	enum class CameraType
	{
		TOP_DOWN_FOLLOW,
		THIRD_PERSON_FOLLOW,
		FIRST_PERSON,
	};

	void SetFollowTransform(Uknitty::Transform* followTransform) { m_followTransform = followTransform; }
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
	const float MOUSE_SENSITIVITY = 0.05f;

	//const glm::vec3 FOLLOW_OFFSET_TOPDOWN = glm::vec3(0, 12, -8);
	const glm::vec3 FOLLOW_OFFSET_TOPDOWN = glm::vec3(0, 30, -4);

	const float FOLLOW_OFFSET_Y_THIRD_PERSON = 2.0f;
	const float FOLLOW_DISTANCE_THIRD_PERSON = 5.0f;
	const float PITCH_MAX_THIRD_PERSON = 10.0f;
	const float PITCH_MIN_THIRD_PERSON = -45.0f;

	const glm::vec3 FOLLOW_OFFSET_FIRST_PERSON = glm::vec3(0, 2, 0.2);
	const float FOLLOW_OFFSET_Y_FIRST_PERSON = 2.0f;
	const float FOLLOW_DISTANCE_FIRST_PERSON = 0.5f;
	const float YAW_MAX_FIRST_PERSON = 0.0f;
	const float YAW_MIN_FIRST_PERSON = -180.0f;

	const Key FORWARD_KEY = Key::W;
	const Key BACKWARD_KEY = Key::S;
	const Key RIGHT_KEY = Key::D;
	const Key LEFT_KEY = Key::A;
	const Key TYPE_SWITCH_KEY = Key::V;

	// variables
	CameraType m_cameraType = CameraType::TOP_DOWN_FOLLOW;
	Uknitty::Transform* m_followTransform = nullptr;

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

	bool m_isForwardKeyDown = false;
	bool m_isBackwardKeyDown = false;
	bool m_isLeftKeyDown = false;
	bool m_isRightKeyDown = false;
};
