#pragma once

#include "IInput.h"
#include "IInputKey.h"
#include "IInputProcessor.h"
#include "ILifeCycle.h"

#include <glm/glm.hpp>

namespace Uknitty
{

class ICamera :
	public IInputProcessor,
	public ILifeCycle
{
public:
	virtual ~ICamera() = default;

	virtual void GetCameraProperties(glm::vec3& pos, glm::vec3& front, glm::vec3& up) const = 0;
	virtual float GetFov() const = 0;
	virtual glm::mat4 GetView() = 0;
	virtual glm::mat4 GetProjection() = 0;
	virtual glm::vec3 GetForward() = 0;
	virtual glm::vec3 GetRight() = 0;

	// Inherited via IInputProcessor
	virtual void MouseCallback(double xPos, double yPos) = 0;
	virtual void KeyDown(Key key) = 0;
	virtual void KeyUp(Key key) = 0;
	virtual void ProcessInput(IKeyboard* iKeyboard) = 0;

	// Inherited via ILifeCycle
	virtual void Awake() override {};
	virtual void Start() override {};
	virtual void Update(float deltaTime) override = 0;
	virtual void LateUpdate(float deltaTime) override = 0;
	virtual void FixedUpdate() override {};

	const int TOTAL_CAMERA_TYPES = 3;
	enum class CameraType
	{
		TOP_DOWN_FOLLOW,
		THIRD_PERSON_FOLLOW,
		FIRST_PERSON,
	};
	CameraType GetCameraType() const { return m_cameraType; }

protected:
	CameraType m_cameraType = CameraType::TOP_DOWN_FOLLOW;
};

}
