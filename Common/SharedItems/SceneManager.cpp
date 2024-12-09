#include "SceneManager.h"

#include "btBulletDynamicsCommon.h"
#include "common.h"
#include "GeneralCamera.h"
#include "InterfaceManager.h"
#include "Model.h"
#include "Player.h"
#include "Scene.h"
#include "ShaderProgram.h"
#include <BTDebugDraw.h>

void SceneManager::ProcessMousePosition(double xPos, double yPos)
{
	m_interfaceManager->ProcessMousePosition(xPos, yPos);
}

void SceneManager::ProcessKeyboard(IKeyboard* iKeyboard)
{
	m_interfaceManager->ProcessKeyboard(iKeyboard);
}

void SceneManager::KeyDown(Key key)
{
	if(key == CHANGE_SCENE_KEY)
	{
		m_currentScene->Destroy();
		m_interfaceManager->RemoveFlowInputRender(m_currentScene);
		m_currentMapId = m_currentMapId == 1 ? 2 : 1;
		LoadScene(m_currentMapId);
		m_interfaceManager->AddFlowInputRender(m_currentScene);
		m_currentScene->Awake();
		m_currentScene->Start();
	}

	else
	{
		m_interfaceManager->KeyDown(key);
	}
}

void SceneManager::KeyUp(Key key)
{
	m_interfaceManager->KeyUp(key);
}

void SceneManager::Awake()
{
	m_interfaceManager = new Uknitty::InterfaceManager();
}

void SceneManager::Start()
{
	CreateCamera();
	m_interfaceManager->AddFlowInput(m_camera);

	CreateShaderProgram();

	CreatePlayer();
	m_interfaceManager->AddFlowInputRender(m_player);

	static_cast<GeneralCamera*>(m_camera)->SetFollowTransform(m_player->m_transform);

	CreatePhysicsWorld();

	LoadScene(INITIAL_MAP_ID);
	m_interfaceManager->AddFlowInputRender(m_currentScene);

	m_interfaceManager->Awake();
	m_interfaceManager->Start();
}

void SceneManager::Update(float deltaTime)
{
	UpdatePhysics();
	m_interfaceManager->Update(deltaTime);
}

void SceneManager::LateUpdate(float deltaTime)
{
	m_interfaceManager->LateUpdate(deltaTime);
}

void SceneManager::FixedUpdate()
{
	m_interfaceManager->FixedUpdate();
}

void SceneManager::Destroy()
{
	std::cout << "Destroying SceneManager" << std::endl;
	DestroyPhysics();
	m_interfaceManager->Destroy();
	delete m_shaderProgram;
	delete m_snakeModel;
	delete this;
}

void SceneManager::Draw()
{
	m_interfaceManager->Draw();
	m_btDynamicsWorld->debugDrawWorld();
}

void SceneManager::LoadScene(int mapId)
{
	m_currentScene = new Scene(mapId, m_camera, m_shaderProgram, m_player);
}

void SceneManager::CreatePlayer()
{
	m_snakeModel = new Uknitty::Model("../Common/Assets/Models/NakedSnake/NakedSnake.obj");
	m_player = new Player(m_snakeModel, m_camera, m_shaderProgram);
}

void SceneManager::CreateCamera()
{
	m_camera = new GeneralCamera();
}

void SceneManager::CreateShaderProgram()
{
	m_shaderProgram = new Uknitty::ShaderProgram("../Common/Assets/Shaders/Vertex.glsl", "../Common/Assets/Shaders/Fragment.glsl");
}

void SceneManager::CreatePhysicsWorld()
{
#pragma region Bullet Initialization
	//TODO delete all these news
	///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
	m_btCollisionConfiguration = new btDefaultCollisionConfiguration();
	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	m_btDispatcher = new btCollisionDispatcher(m_btCollisionConfiguration);
	///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	m_btBroadphase = new btDbvtBroadphase();
	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	m_btSolver = new btSequentialImpulseConstraintSolver;
	m_btDynamicsWorld = new btDiscreteDynamicsWorld(m_btDispatcher, m_btBroadphase, m_btSolver, m_btCollisionConfiguration);
	m_btDynamicsWorld->setGravity(btVector3(0, -9.81, 0));

	m_btDebugDrawer = new Uknitty::BTDebugDraw(m_camera);
	m_btDebugDrawer->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
	m_btDynamicsWorld->setDebugDrawer(m_btDebugDrawer);
#pragma endregion Bullet Initialization

#pragma region Ground Static RigidBody
	{
		//the ground is a cube of side 100 at position y = -56.
		//the sphere will hit it at y = -6, with center at -5
		btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(MAP_SCALE_X / 2.0), btScalar(1), btScalar(MAP_SCALE_Z / 2.0)));

		m_btCollisionShapes.push_back(groundShape);

		btTransform groundTransform;
		groundTransform.setIdentity();
		groundTransform.setOrigin(btVector3(0, -1, 0));

		btScalar mass(0.);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0, 0, 0);
		if(isDynamic)
			groundShape->calculateLocalInertia(mass, localInertia);

		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);

		//add the body to the dynamics world
		m_btDynamicsWorld->addRigidBody(body);
	}

	{
		//the ground is a cube of side 100 at position y = -56.
		//the sphere will hit it at y = -6, with center at -5
		btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(16), btScalar(10), btScalar(2)));

		m_btCollisionShapes.push_back(groundShape);

		btTransform groundTransform;
		groundTransform.setIdentity();
		groundTransform.setOrigin(btVector3(0, 0, 12));

		btScalar mass(0.);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0, 0, 0);
		if(isDynamic)
			groundShape->calculateLocalInertia(mass, localInertia);

		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);

		//add the body to the dynamics world
		m_btDynamicsWorld->addRigidBody(body);
	}
#pragma endregion

#pragma region Sphere Dynamic RigidBody
	{
		//create a dynamic rigidbody

		//btCollisionShape* colShape = new btBoxShape(btVector3(1,1,1));
		btCollisionShape* colShape = new btSphereShape(btScalar(1.));
		m_btCollisionShapes.push_back(colShape);

		/// Create Dynamic Objects
		btTransform startTransform;
		startTransform.setIdentity();

		btScalar mass(1.f);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0, 0, 0);
		if(isDynamic)
			colShape->calculateLocalInertia(mass, localInertia);

		startTransform.setOrigin(btVector3(0, 40, 0));

		//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);

		m_btDynamicsWorld->addRigidBody(body);
	}
#pragma endregion

}

void SceneManager::UpdatePhysics()
{
	m_btDynamicsWorld->stepSimulation(1.0f / 60.0f, 10);
	btCollisionObject* obj = m_btDynamicsWorld->getCollisionObjectArray()[2];
	//print positions of all objects
	for(int j = m_btDynamicsWorld->getNumCollisionObjects() - 1; j >= 0; j--)
	{
		btCollisionObject* obj = m_btDynamicsWorld->getCollisionObjectArray()[j];
		btRigidBody* body = btRigidBody::upcast(obj);
		if(j == m_btDynamicsWorld->getNumCollisionObjects() - 1)
		{
			body->setLinearVelocity(btVector3(1.0, body->getLinearVelocity().getY(), 1.0));
		}
		btTransform trans;
		if(body && body->getMotionState())
		{
			body->getMotionState()->getWorldTransform(trans);
		}
		else
		{
			trans = obj->getWorldTransform();
		}
		//printf("world pos object %d = %f,%f,%f\n", j, float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
	}

	/*int numManifolds = m_btDynamicsWorld->getDispatcher()->getNumManifolds();
	for(int i = 0; i < numManifolds; i++)
	{
		btPersistentManifold* contactManifold = m_btDynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
		const btCollisionObject* obA = contactManifold->getBody0();
		const btCollisionObject* obB = contactManifold->getBody1();

		int numContacts = contactManifold->getNumContacts();
		for(int j = 0; j < numContacts; j++)
		{
			btManifoldPoint& pt = contactManifold->getContactPoint(j);
			if(pt.getDistance() < 0.f)
			{
				const btVector3& ptA = pt.getPositionWorldOnA();
				const btVector3& ptB = pt.getPositionWorldOnB();
				const btVector3& normalOnB = pt.m_normalWorldOnB;
				std::cout << "**************************************\n************************\n***********************\n\n\n\n";
			}
		}
	}*/

}

void SceneManager::DestroyPhysics()
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

	//delete dynamics world
	delete m_btDynamicsWorld;

	//delete solver
	delete m_btSolver;

	//delete broadphase
	delete m_btBroadphase;

	//delete dispatcher
	delete m_btDispatcher;

	//delete collision configuration
	delete m_btCollisionConfiguration;

	//next line is optional: it will be cleared by the destructor when the array goes out of scope
	m_btCollisionShapes.clear();
}
