#pragma once

#include "btBulletDynamicsCommon.h"
#include <unordered_map>

namespace Uknitty
{

class Physics;

class CollisionManager : public btCollisionWorld::ContactResultCallback
{
private:
	std::unordered_map<const btCollisionObject*, Uknitty::Physics*> listeners;

public:
	void registerListener(const btCollisionObject* obj, Uknitty::Physics* listener)
	{
		listeners[obj] = listener;
	}

	void unregisterListener(const btCollisionObject* obj)
	{
		listeners.erase(obj);
	}

	// Detect and handle collisions
	btScalar addSingleResult(btManifoldPoint& cp,
							 const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0,
							 const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1) override;
};

} // namespace Uknitty
