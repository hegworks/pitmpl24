#pragma once

#include "IInput.h"
#include <X11/Xlib.h>

class XWindow;

class RaspMouse : public IMouse
{
public:
	RaspMouse(Display& display, Window& window);

	virtual bool GetButtonDown(MouseButton button) const override;
	virtual glm::vec2 GetPosition() override;
	virtual float GetScrollDelta() const override;

	glm::ivec2 m_resettedPosOffset = glm::ivec2(0);

protected:
	virtual void OnCaptureMouseInput() override;
	virtual void OnReleaseMouseInput() override;

private:
	Display& m_display;
	Window& m_window;

	int m_originalMouseX = 0;
	int m_originalMouseY = 0;
	bool m_shouldChangeGrabState = false;

	void HideCursor();
	void ShowCursor();
};
