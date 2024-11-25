#include "RaspMouse.h"
#include <Common.h>
#include <iostream>

RaspMouse::RaspMouse(Display& display, Window& window) :
	display(display),
	window(window)
{
	XWarpPointer(&display, None, window, 0, 0, 0, 0, SCRWIDTH / 2.0, SCRHEIGHT / 2.0);
	XFlush(&display); // Ensure the command is sent immediately
}

bool RaspMouse::GetButtonDown(MouseButtons button) const
{
	int root_x;
	int root_y;
	int win_x;
	int win_y;
	uint mask_return;
	Window window_returned;
	XQueryPointer(
		&display,
		window,
		&window_returned,
		&window_returned,

		&root_x,
		&root_y,
		&win_x,
		&win_y,
		&mask_return);

	switch(button)
	{
		case MouseButtons::LEFT:
		{
			return mask_return & Button1MotionMask;
		}
		case MouseButtons::RIGHT:
		{
			return mask_return & Button3MotionMask;
		}
		case MouseButtons::MIDDLE:
		{
			return mask_return & Button2MotionMask;
		}
		default:
		{
			return false;
		}
	}
}

glm::vec2 RaspMouse::GetPosition()
{
	int root_x;
	int root_y;
	int win_x;
	int win_y;
	uint mask_return;
	Window window_returned;
	XQueryPointer(
		&display,
		window,
		&window_returned,
		&window_returned,

		&root_x,
		&root_y,
		&win_x,
		&win_y,
		&mask_return);

	// Center of the screen
	glm::ivec2 center(SCRWIDTH / 2, SCRHEIGHT / 2);

	// Check if the mouse is near the edges of the window
	if(win_x <= 1 || win_y <= 1 || win_x >= SCRWIDTH - 1 || win_y >= SCRHEIGHT - 1)
	{
		glm::ivec2 posBeforeReset = glm::ivec2(win_x, win_y);

		// Warp pointer back to the center
		XWarpPointer(&display, None, window, 0, 0, 0, 0, center.x, center.y);
		XFlush(&display); // Ensure the warp is sent immediately

		// Update the offset to account for the warp
		m_resettedPosOffset += posBeforeReset - center;

		// Return the "virtual" position based on the offset
		return glm::vec2(center.x + m_resettedPosOffset.x, center.y + m_resettedPosOffset.y);
	}

	// Return the adjusted mouse position
	return glm::vec2(win_x + m_resettedPosOffset.x, win_y + m_resettedPosOffset.y);
}

float RaspMouse::GetScrollDelta() const
{
	return 0;
}
