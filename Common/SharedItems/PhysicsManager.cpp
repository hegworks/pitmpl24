#include "PhysicsManager.h"

#include "CollisionManager.h"
#include "UknittySettings.h"

namespace Uknitty
{

PhysicsManager::PhysicsManager()
{
#pragma region Bullet Initialization
	///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
	m_btCollisionConfiguration = new btDefaultCollisionConfiguration();
	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	m_btDispatcher = new btCollisionDispatcher(m_btCollisionConfiguration);
	///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	m_btBroadphase = new btDbvtBroadphase();
	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	m_btSolver = new btSequentialImpulseConstraintSolver;
	m_btDynamicsWorld = new btDiscreteDynamicsWorld(m_btDispatcher, m_btBroadphase, m_btSolver, m_btCollisionConfiguration);
	m_btDynamicsWorld->setGravity(btVector3(0, GRAVITY, 0));

#ifdef DEBUG_DRAW_PHYSICS
	m_btDebugDrawer = new Uknitty::BTDebugDraw();
	m_btDebugDrawer->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
	m_btDynamicsWorld->setDebugDrawer(m_btDebugDrawer);
#endif // DEBUG_DRAW_PHYSICS

	m_collisionManager = new Uknitty::CollisionManager();
#pragma endregion Bullet Initialization
}

PhysicsManager::~PhysicsManager()
{
	for(int i = m_btDynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = m_btDynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if(body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		m_btDynamicsWorld->removeCollisionObject(obj);
		delete obj;
	}

	//delete collision shapes
	for(int j = 0; j < m_btCollisionShapes.size(); j++)
	{
		btCollisionShape* shape = m_btCollisionShapes[j];
		m_btCollisionShapes[j] = 0;
		delete shape;
	}

	delete m_btDebugDrawer;
	delete m_btDynamicsWorld;
	delete m_btSolver;
	delete m_btBroadphase;
	delete m_btDispatcher;
	delete m_btCollisionConfiguration;
	m_btCollisionShapes.clear();
}

void PhysicsManager::Update(float deltaTime)
{
#if 0 //TODO GOH find a replacemet for this
	if(m_isNewSceneLoading) return;
#endif

	m_btDynamicsWorld->stepSimulation(deltaTime, 10, PHYSICS_TIMESTEP);
	m_btDynamicsWorld->performDiscreteCollisionDetection();
	for(auto rigidbody : m_contactTestRigidbodies)
	{
		m_btDynamicsWorld->contactTest(rigidbody, *m_collisionManager);
	}
}

//TODO GOH use this on player
void PhysicsManager::AddContactTestRigidbody(btRigidBody* rigidbody)
{
	m_contactTestRigidbodies.push_back(rigidbody);
}

void PhysicsManager::RemoveContactTestRigidbody(btRigidBody* rigidbody)
{
	auto it = std::find(m_contactTestRigidbodies.begin(), m_contactTestRigidbodies.end(), rigidbody);
	if(it != m_contactTestRigidbodies.end())
	{
		m_contactTestRigidbodies.erase(it);
	}
}

void PhysicsManager::RegisterListener(btRigidBody* rigidbody, Uknitty::CPhysics* cphysics)
{
	m_collisionManager->RegisterListener(rigidbody, cphysics);
}

void PhysicsManager::UnregisterListener(btRigidBody* rigidbody)
{
	m_collisionManager->UnregisterListener(rigidbody);
}

} // namespace Uknitty
