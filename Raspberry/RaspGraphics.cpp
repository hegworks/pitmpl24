#include "RaspGraphics.h"
#include <EGL/egl.h>
#include "EGLState.h"
#include "RaspMouse.h"
#include "XWindow.h"

RaspGraphics::RaspGraphics() : m_window(new XWindow())
{
	m_window->CreateWindow();
}

void RaspGraphics::Quit()
{
	XDestroyWindow(&m_window->GetDisplay(), m_window->GetWindow());
}

void RaspGraphics::SwapBuffer()
{
	EGLState state = m_window->GetState();
	eglSwapBuffers(state.display, state.surface);
}

XWindow& RaspGraphics::Window() const
{
	return *m_window;
}
