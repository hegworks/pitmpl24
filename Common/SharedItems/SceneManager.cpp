#include "SceneManager.h"

#include "btBulletDynamicsCommon.h"
#include "CollisionManager.h"
#include "Common.h"
#include "GeneralCamera.h"
#include "InterfaceManager.h"
#include "Model.h"
#include "Physics.h"
#include "Player.h"
#include "RoomFinder.h"
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
	/*if(key == CHANGE_SCENE_KEY)
	{
		m_currentScene->Destroy();
		m_interfaceManager->RemoveFlowInputRender(m_currentScene);
		m_currentMapId = m_currentMapId == 1 ? 2 : 1;
		LoadScene(m_currentMapId);
		m_interfaceManager->AddFlowInputRender(m_currentScene);
		m_currentScene->Awake();
		m_currentScene->Start();
	}*/

	//else
	//{
	m_interfaceManager->KeyDown(key);
	//}
}

void SceneManager::KeyUp(Key key)
{
	m_interfaceManager->KeyUp(key);
}

void SceneManager::Awake()
{
	m_interfaceManager = new Uknitty::InterfaceManager();
	m_roomFinder = new RoomFinder();
}

void SceneManager::Start()
{
	CreateCamera();
	m_interfaceManager->AddFlowInput(m_camera);

	CreateShaderProgram();

	CreatePhysicsWorld();

	CreatePlayer();
	m_collisionManager->registerListener(m_player->GetPhysics()->GetRigidBody(), m_player->GetPhysics());
	m_interfaceManager->AddFlowInputRender(m_player);
	m_player->SetCollidedWithRoomChangeCallback([this](RoomChangeType roomChangeType) { OnPlayerCollidedWithRoomChange(roomChangeType); });

	static_cast<GeneralCamera*>(m_camera)->SetFollowTransform(m_player->m_transform);

	LoadScene(m_roomFinder->GetCurrentLevelId());
	m_interfaceManager->AddFlowInputRender(m_currentScene);

	m_interfaceManager->Awake();
	m_interfaceManager->Start();
}

void SceneManager::Update(float deltaTime)
{
	UpdatePhysics(deltaTime);
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
	m_interfaceManager->Destroy();
	DestroyPhysics();
	delete m_shaderProgram;
	delete m_snakeModel;
	delete this;
}

void SceneManager::Draw()
{
	m_interfaceManager->Draw();
#ifdef DEBUG_DRAW
	m_btDynamicsWorld->debugDrawWorld();
#endif // DEBUG_DRAW
}

void SceneManager::OnPlayerCollidedWithRoomChange(RoomChangeType roomChangeType)
{
	std::cout << "Player collided with room change: " << static_cast<int>(roomChangeType) << std::endl;
	RoomChange* newRoomChange = m_roomFinder->FindNextRoom(roomChangeType);
	int newMapId = newRoomChange->nextRoomId;
	m_isNewSceneLoading = true;
	ChangeScene(newMapId);
	m_isNewSceneLoading = false;
	m_roomFinder->SetCurrentLevelId(newMapId);
	m_player->RoomChangedSetPosition(newRoomChange);
}

void SceneManager::LoadScene(int mapId)
{
	m_currentScene = new Scene(mapId, m_camera, m_shaderProgram, m_player, m_btDynamicsWorld);
}

void SceneManager::ChangeScene(int mapId)
{
	m_currentScene->Destroy();
	m_interfaceManager->RemoveFlowInputRender(m_currentScene);
	m_currentMapId = mapId;
	LoadScene(m_currentMapId);
	m_interfaceManager->AddFlowInputRender(m_currentScene);
	m_currentScene->Awake();
	m_currentScene->Start();
}

void SceneManager::CreatePlayer()
{
	m_snakeModel = new Uknitty::Model("../Common/Assets/Models/NakedSnake/NakedSnake.obj");
	m_player = new Player(m_snakeModel, m_camera, m_shaderProgram, m_btDynamicsWorld);
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

#ifdef DEBUG_DRAW
	m_btDebugDrawer = new Uknitty::BTDebugDraw(m_camera);
	m_btDebugDrawer->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
	m_btDynamicsWorld->setDebugDrawer(m_btDebugDrawer);
#endif // DEBUG_DRAW

	m_collisionManager = new Uknitty::CollisionManager();
#pragma endregion Bullet Initialization

	/*
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

	#ifdef WINDOWS_BUILD
			m_btDynamicsWorld->DebugAddRigidBody(body, "sphere");
	#elif Raspberry_BUILD
			m_btDynamicsWorld->addRigidBody(body);
	#endif
		}
	#pragma endregion
	*/
}

void SceneManager::UpdatePhysics(float deltaTime)
{
	if(m_isNewSceneLoading) return;

	m_btDynamicsWorld->stepSimulation(deltaTime, 1.0f / 30.0f, 10);
	m_btDynamicsWorld->performDiscreteCollisionDetection();
	m_btDynamicsWorld->contactTest(m_player->GetPhysics()->GetRigidBody(), *m_collisionManager);

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

	delete m_btDebugDrawer;
	delete m_btDynamicsWorld;
	delete m_btSolver;
	delete m_btBroadphase;
	delete m_btDispatcher;
	delete m_btCollisionConfiguration;
	m_btCollisionShapes.clear();
}
