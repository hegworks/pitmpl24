#pragma once

#include "GameObject.h"

namespace Uknitty
{
class LightManager;


class LightSource : public GameObject
{
public:
	virtual void OnAwake() override;

private:
	LightManager* m_lightManager = nullptr;
};

} // namespace Uknitty