#pragma once

#include "GameObject.h"
#include "glm/glm.hpp"

class btDynamicsWorld;

class SolidObject : public Uknitty::GameObject
{
public:
	virtual void OnAwake() override;

private:
	btDynamicsWorld* m_btDynamicsWorld = nullptr;
};
