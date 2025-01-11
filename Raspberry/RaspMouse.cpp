#include "RaspMouse.h"

#include "UknittySettings.h"
#include "XWindow.h"
#include <iostream>

RaspMouse::RaspMouse(Display& display, Window& window) :
	m_display(display),
	m_window(window)
{
	XWarpPointer(&display, None, window, 0, 0, 0, 0, Uknitty::SCRWIDTH / 2.0, Uknitty::SCRHEIGHT / 2.0);
	XFlush(&display); // Ensure the command is sent immediately

	ReleaseMouseInput();
}

bool RaspMouse::GetButtonDown(MouseButton button) const
{
	int root_x;
	int root_y;
	int win_x;
	int win_y;
	uint mask_return;
	Window window_returned;
	XQueryPointer(
		&m_display,
		m_window,
		&window_returned,
		&window_returned,

		&root_x,
		&root_y,
		&win_x,
		&win_y,
		&mask_return);

	switch(button)
	{
		case MouseButton::LEFT:
		{
			return mask_return & Button1MotionMask;
		}
		case MouseButton::RIGHT:
		{
			return mask_return & Button3MotionMask;
		}
		case MouseButton::MIDDLE:
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
		&m_display,
		m_window,
		&window_returned,
		&window_returned,

		&root_x,
		&root_y,
		&win_x,
		&win_y,
		&mask_return);

	if(!m_isCapturingMouseInput)
	{
		return glm::vec2(win_x, win_y);
	}

	glm::ivec2 center(Uknitty::SCRWIDTH / 2, Uknitty::SCRHEIGHT / 2);
	// Check if the mouse is near the edges of the window
	if(win_x <= 1 || win_y <= 1 || win_x >= Uknitty::SCRWIDTH - 1 || win_y >= Uknitty::SCRHEIGHT - 1)
	{
		glm::ivec2 posBeforeReset = glm::ivec2(win_x, win_y);

		// Warp pointer back to the center
		XWarpPointer(&m_display, None, m_window, 0, 0, 0, 0, center.x, center.y);
		XFlush(&m_display); // Ensure the warp is sent immediately

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

void RaspMouse::OnCaptureMouseInput()
{
	// Store the mouse's original position before warping
	int rootX, rootY, winX, winY;
	unsigned int mask;
	Window rootReturn, childReturn;
	XQueryPointer(&m_display, m_window, &rootReturn, &childReturn, &rootX, &rootY, &winX, &winY, &mask);
	m_originalMouseX = winX;
	m_originalMouseY = winY;
	m_resettedPosOffset = glm::ivec2(0);

	HideCursor();

	// Warp the mouse to the center of the screen
	XWarpPointer(&m_display, None, m_window, 0, 0, 0, 0, Uknitty::SCRWIDTH / 2, Uknitty::SCRHEIGHT / 2);
	XFlush(&m_display); // Ensure the command is sent immediately
}

void RaspMouse::OnReleaseMouseInput()
{
	m_resettedPosOffset = glm::ivec2(0);

	ShowCursor();

	// Restore the original mouse position
	XWarpPointer(&m_display, None, m_window, 0, 0, 0, 0, m_originalMouseX, m_originalMouseY);
	XFlush(&m_display); // Ensure the command is sent immediately
}

void RaspMouse::HideCursor()
{
	Cursor invisibleCursor;
	Pixmap bitmapNoData;
	XColor black;
	static char noData[] = {0, 0, 0, 0, 0, 0, 0, 0};
	black.red = black.green = black.blue = 0;
	bitmapNoData = XCreateBitmapFromData(&m_display, m_window, noData, 8, 8);
	invisibleCursor = XCreatePixmapCursor(&m_display, bitmapNoData, bitmapNoData, &black, &black, 0, 0);
	XDefineCursor(&m_display, m_window, invisibleCursor);
	XFreeCursor(&m_display, invisibleCursor);
	XFreePixmap(&m_display, bitmapNoData);

	XGrabPointer(&m_display, m_window, True, 0, GrabModeAsync, GrabModeAsync, m_window, None, CurrentTime);
}

void RaspMouse::ShowCursor()
{
	XUndefineCursor(&m_display, m_window);
	XUngrabPointer(&m_display, CurrentTime);
}
