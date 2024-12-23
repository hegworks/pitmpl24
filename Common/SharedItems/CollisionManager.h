#pragma once

#include "btBulletDynamicsCommon.h"
#include <unordered_map>

namespace Uknitty
{

class CPhysics;

class CollisionManager : public btCollisionWorld::ContactResultCallback
{
private:
	std::unordered_map<const btCollisionObject*, Uknitty::CPhysics*> listeners;

public:
	void RegisterListener(const btCollisionObject* obj, Uknitty::CPhysics* listener)
	{
		listeners[obj] = listener;
	}

	void UnregisterListener(const btCollisionObject* obj)
	{
		listeners.erase(obj);
	}

	// Detect and handle collisions
	virtual btScalar addSingleResult(btManifoldPoint& cp,
									 const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0,
									 const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1) override;
};

} // namespace Uknitty
