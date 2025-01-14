#pragma once

#include "CInput.h"
#include "UknittySettings.h"

namespace Uknitty
{

class GeneralCamera;

class GeneralCameraCInput : public Uknitty::CInput
{
public:
	GeneralCameraCInput(GeneralCamera* owner);

	virtual void ProcessMousePosition(double xPos, double yPos) override;
	virtual void OnKeyDown(Key key) override;

	void ResetMouseOffset();

private:
	GeneralCamera* m_owner = nullptr;

	bool m_isFirstMouse = true;
	double m_lastX = SCRWIDTH / 2.0;
	double m_lastY = SCRHEIGHT / 2.0;
	bool m_isResetMouse = false;
};

} // namespace Uknitty
