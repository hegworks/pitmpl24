#include "CollisionManager.h"

#include "CPhysics.h"

btScalar Uknitty::CollisionManager::addSingleResult([[maybe_unused]] btManifoldPoint& cp,
													const btCollisionObjectWrapper* colObj0Wrap, [[maybe_unused]] int partId0, [[maybe_unused]] int index0,
													const btCollisionObjectWrapper* colObj1Wrap, [[maybe_unused]] int partId1, [[maybe_unused]] int index1)
{
	const btCollisionObject* obj0 = colObj0Wrap->getCollisionObject();
	const btCollisionObject* obj1 = colObj1Wrap->getCollisionObject();

	if(listeners.count(obj0))
	{
		listeners[obj0]->OnCollision(obj1);
	}
	if(listeners.count(obj1))
	{
		listeners[obj1]->OnCollision(obj0);
	}

	return 0; // Not used
}
