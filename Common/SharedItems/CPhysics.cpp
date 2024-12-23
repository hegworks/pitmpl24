#include "CPhysics.h"

#include "btBulletDynamicsCommon.h"
#include <glm/glm.hpp>
#include <stdexcept>

namespace Uknitty
{

CPhysics::CPhysics()
{
}

CPhysics::~CPhysics()
{
	delete m_rigidBody;
	delete m_collisionShape;
	delete m_motionState;
}

void CPhysics::InitialzeWithBoxShape(glm::vec3 position, glm::vec3 scale, float mass)
{
	btCollisionShape* shape = new btBoxShape(btVector3(btScalar(scale.x / 2.0), btScalar(scale.y / 2.0), btScalar(scale.z / 2.0)));
	Initialze(shape, position, scale, mass);
}

void CPhysics::InitialzeWithCapsuleShape(glm::vec3 position, float radius, float height, float mass)
{
	btCollisionShape* shape = new btCapsuleShape(radius, height);
	Initialze(shape, position, glm::vec3(radius, height, radius), mass);
}

void CPhysics::Initialze(btCollisionShape* btCollisionShape, glm::vec3 position, glm::vec3 scale, float mass)
{
	m_collisionShape = btCollisionShape;

	btTransform btTransform;
	btTransform.setIdentity();
	btTransform.setOrigin(btVector3(position.x, position.y + (scale.y / 2.0), position.z));
	m_motionState = new btDefaultMotionState(btTransform);

	btVector3 localInertia(0, 0, 0);
	// if mass is 0, then the object is static (no need to calculate inertia)
	if(mass > 0.0f)
	{
		m_collisionShape->calculateLocalInertia(mass, localInertia);
	}
	else if(mass < 0.0f)
	{
		throw std::runtime_error("mass can't be negative");
	}

	btRigidBody::btRigidBodyConstructionInfo rigidBodyInfo(mass, m_motionState, m_collisionShape, localInertia);
	m_rigidBody = new btRigidBody(rigidBodyInfo);
}

} // namespace Uknitty
