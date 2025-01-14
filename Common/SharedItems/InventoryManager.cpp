#include "InventoryManager.h"

#include "AssetManager.h"
#include "CameraObject.h"
#include "CTransform.h"
#include "Engine.h"
#include "GameSharedDependencies.h"
#include "glm/glm.hpp"
#include "ModelDataStorage.h"
#include "ModelObject.h"
#include "string"
#include <GameSettings.h>
#include <vector>

InventoryManager::InventoryManager()
{
	ModelDataStorage* modelDataStorage = GameSharedDependencies::Get<ModelDataStorage>();
	Uknitty::Engine* engine = Uknitty::Engine::GetInstance();
	Uknitty::AssetManager* assetManager = engine->GetAssetManager();
	for(auto& name : m_modelObjectNames)
	{
		ModelDataStorage::ModelData* modelData = modelDataStorage->GetModelData(name);
		Uknitty::ModelObject* modelObject = engine->CreateGameObject<Uknitty::ModelObject>();
		modelObject->Initialize(assetManager->AutoGetModel(ModelDataStorage::INVENTORY_GUN, modelData->m_filePath), assetManager->AutoGetShaderProgram(MAIN_SHADERPROGRAM));
		modelObject->GetLocalTransform()->SetScale(glm::vec3(0.01));
		modelObject->GetLocalTransform()->SetPosition(glm::vec3(0, 0.04, -0.2));
		modelObject->SetParent(engine->GetMainCamera());
		m_modelObjects.push_back(modelObject);
	}
	HideInventory();
}

InventoryManager::~InventoryManager()
{
}

void InventoryManager::Update(float deltaTime)
{
	for(auto& modelObject : m_modelObjects)
	{
		glm::vec3 rot = *modelObject->GetLocalTransform()->GetRotation();
		rot.y += 60 * deltaTime;
		modelObject->GetLocalTransform()->SetRotation(rot);
	}
}

void InventoryManager::ShowInventory()
{
	for(auto& modelObject : m_modelObjects)
	{
		modelObject->EnableDrawSelf();
	}
	m_isVisible = false;
}

void InventoryManager::HideInventory()
{
	for(auto& modelObject : m_modelObjects)
	{
		modelObject->DisableDrawSelf();
	}
	m_isVisible = false;
}
