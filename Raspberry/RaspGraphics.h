#pragma once
#include "IGraphics.h"

class XWindow;
struct EGLState;

class RaspGraphics : public IGraphics
{
public:
	RaspGraphics();
	~RaspGraphics() {};

	virtual void Quit();
	virtual void SwapBuffer();
	XWindow& Window() const;

private:
	XWindow* m_window = nullptr;
};
