#pragma once
#include "IInput.h"
#include <X11/Xlib.h>

class RaspMouse : public IMouse
{
public:
	RaspMouse(Display& display, Window& window);

	bool GetButtonDown(MouseButtons button) const override;
	glm::vec2 GetPosition() override;
	float GetScrollDelta() const override;

	glm::ivec2 m_resettedPosOffset = glm::ivec2(0);

private:
	Display& display;
	Window& window;
};
