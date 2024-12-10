#include "Physics.h"

#include "btBulletDynamicsCommon.h"
#include <glm/glm.hpp>
#include <stdexcept>

Uknitty::Physics::Physics()
{
}

Uknitty::Physics::~Physics()
{
	delete m_rigidBody;
	delete m_collisionShape;
	delete m_motionState;
}

void Uknitty::Physics::InitialzeWithBoxShape(glm::vec3 position, glm::vec3 scale, float mass)
{
	btCollisionShape* shape = new btBoxShape(btVector3(btScalar(scale.x / 2.0), btScalar(scale.y / 2.0), btScalar(scale.z / 2.0)));
	Initialze(shape, position, scale, mass);
}

void Uknitty::Physics::InitialzeWithCapsuleShape(glm::vec3 position, float radius, float height, float mass)
{
	btCollisionShape* shape = new btCapsuleShape(radius, height);
	Initialze(shape, position, glm::vec3(radius, height, radius), mass);
}

void Uknitty::Physics::Initialze(btCollisionShape* btCollisionShape, glm::vec3 position, glm::vec3 scale, float mass)
{
	m_collisionShape = btCollisionShape;

	btTransform btTransform;
	btTransform.setIdentity();
	btTransform.setOrigin(btVector3(position.x, position.y + (scale.y / 2.0), position.z));

	btVector3 localInertia(0, 0, 0);
	if(mass > 0.0f)
	{
		m_collisionShape->calculateLocalInertia(mass, localInertia);
	}
	else if(mass < 0.0f)
	{
		throw std::runtime_error("mass can't be negative");
	}
	// if mass is 0, then the object is static

	m_motionState = new btDefaultMotionState(btTransform);
	btRigidBody::btRigidBodyConstructionInfo rigidBodyInfo(mass, m_motionState, m_collisionShape, localInertia);
	m_rigidBody = new btRigidBody(rigidBodyInfo);
}
