#pragma once
// group these 2 togeter

#include <functional> 
#include <glm/vec2.hpp>

#include "IInputKey.h"

class IMouse
{

public:
	virtual bool GetButtonDown(MouseButton button) const = 0;
	virtual glm::vec2 GetPosition() = 0;
	virtual float GetScrollDelta() const = 0;

	virtual ~IMouse() = default;
};

class IKeyboard
{
public:
	virtual bool GetKey(Key key) const = 0;
	virtual ~IKeyboard() = default;

	using KeyCallback = std::function<void(Key key, KeyAction action)>;
	virtual void SetKeyCallback(const KeyCallback& callback) = 0;
};
