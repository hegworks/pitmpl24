#include "GunBullet.h"

#include "glm/glm.hpp"
#include "Model.h"
#include "Physics.h"
#include "PhysicsCollisionFilters.h"
#include <iostream>

GunBullet::GunBullet()
{
	static Uknitty::Model* model = new Uknitty::Model("../Common/Assets/Models/Bullet/Bullet.obj", m_shaderProgram);
	m_model = model;

	m_physics = new Uknitty::Physics();
	m_physics->InitialzeWithBoxShape(INITIAL_POSITION, MODEL_DIMENSIONS, MASS);
	m_physics->GetRigidBody()->setActivationState(DISABLE_DEACTIVATION);
	m_physics->GetRigidBody()->setAngularFactor(btVector3(0, 0, 0)); // lock rotation

	// Enable continuous collision detection
	m_physics->GetRigidBody()->setCcdMotionThreshold(0.01f);     // Motion threshold for CCD
	m_physics->GetRigidBody()->setCcdSweptSphereRadius(0.05f);   // Approximate bullet radius

	DynamicObject::Construct(m_model, INITIAL_POSITION, m_physics);

	m_btDynamicsWorld->addRigidBody(m_physics->GetRigidBody(), COLL_GROUP_BULLET, COLL_MASK_BULLET);
	std::cout << "Adding RigidBody: Bullet" << std::endl;
}

GunBullet::~GunBullet()
{
	delete m_physics;
	delete m_model;
}
