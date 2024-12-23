#pragma once

#include "btBulletDynamicsCommon.h"
#include <vector>

namespace Uknitty
{

class BTDebugDraw;
class CollisionManager;
class CPhysics;

class PhysicsManager
{
public:
	PhysicsManager();
	~PhysicsManager();

	void Update(float deltaTime);
	void AddContactTestRigidbody(btRigidBody* rigidbody);
	void RemoveContactTestRigidbody(btRigidBody* rigidbody);
	void RegisterListener(btRigidBody* rigidbody, Uknitty::CPhysics* cphysics);
	void UnregisterListener(btRigidBody* rigidbody);

private:
	btDefaultCollisionConfiguration* m_btCollisionConfiguration = nullptr;
	btDbvtBroadphase* m_btBroadphase = nullptr;
	btCollisionDispatcher* m_btDispatcher = nullptr;
	btSequentialImpulseConstraintSolver* m_btSolver = nullptr;
	btDiscreteDynamicsWorld* m_btDynamicsWorld = nullptr;
	btAlignedObjectArray<btCollisionShape*> m_btCollisionShapes;

	Uknitty::BTDebugDraw* m_btDebugDrawer = nullptr;
	Uknitty::CollisionManager* m_collisionManager = nullptr;
	std::vector<btRigidBody*> m_contactTestRigidbodies;
};

} // namespace Uknitty
