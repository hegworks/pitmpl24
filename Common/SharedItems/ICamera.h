#pragma once

#include "IInput.h"
#include "IInputKey.h"
#include "IInputProcessor.h"

#include <glm/glm.hpp>

namespace Uknitty
{

class ICamera : public IInputProcessor
{
public:
	ICamera() = default;
	virtual ~ICamera() = default;

	virtual void Update(float deltaTime) = 0;

	virtual void GetCameraProperties(glm::vec3& pos, glm::vec3& front, glm::vec3& up) const = 0;
	virtual float GetFov() const = 0;
	virtual glm::mat4 GetView() = 0;
	virtual glm::mat4 GetProjection() = 0;

	// Inherited via IInputProcessor
	virtual void MouseCallback(double xPos, double yPos) = 0;
	virtual void KeyDown(Key key) = 0;
	virtual void KeyUp(Key key) = 0;
	virtual void ProcessInput(IKeyboard* iKeyboard) = 0;
};

}
