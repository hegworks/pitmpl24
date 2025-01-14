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
	GameSharedDependencies::Set<InventoryManager>(this);

	ModelDataStorage* modelDataStorage = GameSharedDependencies::Get<ModelDataStorage>();
	Uknitty::Engine* engine = Uknitty::Engine::GetInstance();
	Uknitty::AssetManager* assetManager = engine->GetAssetManager();
	for(int i = 0; i < m_modelObjectNames.size(); i++)
	{
		ModelDataStorage::ModelData* modelData = modelDataStorage->GetModelData(m_modelObjectNames[i]);
		Uknitty::ModelObject* modelObject = engine->CreateGameObject<Uknitty::ModelObject>();
		modelObject->Initialize(assetManager->AutoGetModel(m_modelObjectNames[i], modelData->m_filePath), assetManager->AutoGetShaderProgram(MAIN_SHADERPROGRAM));
		modelObject->GetLocalTransform()->SetScale(m_scales[i]);
		modelObject->GetLocalTransform()->SetPosition(m_positions[i]);
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
