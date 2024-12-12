#include "CollisionManager.h"

#include "Physics.h"

btScalar Uknitty::CollisionManager::addSingleResult(btManifoldPoint& cp,
													const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0,
													const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1)
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
