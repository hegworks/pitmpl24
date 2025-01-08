#pragma once
#include "IInput.h"
#include <X11/Xlib.h>

class RaspMouse : public IMouse
{
public:
	RaspMouse(Display& display, Window& window);

	virtual bool GetButtonDown(MouseButton button) const;
	virtual glm::vec2 GetPosition();
	virtual float GetScrollDelta() const;

	glm::ivec2 m_resettedPosOffset = glm::ivec2(0);

private:
	Display& m_display;
	Window& m_window;
};
