#pragma once

#include "CInput.h"
#include "IInputKey.h"
#include <functional>

class IKeyboard;

class PlayerCInput : public Uknitty::CInput
{
public:
	virtual void OnKeyDown(Key key) override;
	virtual void OnKeyUp(Key key) override;
	virtual void OnMouseButtonDown(MouseButton button) override;

	bool IsForwardInput() const { return m_isForwardKeyDown; }
	bool IsBackwardInput() const { return m_isBackwardKeyDown; }
	bool IsLeftInput() const { return m_isLeftKeyDown; }
	bool IsRightInput() const { return m_isRightKeyDown; }

	void SetOnShootInput(std::function<void()> callback) { m_onShootInputCallback = callback; }

private:
	bool m_isForwardKeyDown = false;
	bool m_isBackwardKeyDown = false;
	bool m_isLeftKeyDown = false;
	bool m_isRightKeyDown = false;

	std::function<void()> m_onShootInputCallback;

#pragma region Settings
	const Key FORWARD_KEY = Key::W;
	const Key BACKWARD_KEY = Key::S;
	const Key LEFT_KEY = Key::A;
	const Key RIGHT_KEY = Key::D;
	const MouseButton SHOOT_BUTTON = MouseButton::LEFT;
#pragma endregion
};
