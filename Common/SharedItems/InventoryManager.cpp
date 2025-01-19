#include "InventoryManager.h"

#include "AssetManager.h"
#include "CameraObject.h"
#include "CTransform.h"
#include "Engine.h"
#include "GameplayEvents.h"
#include "GameSharedDependencies.h"
#include "glm/glm.hpp"
#include "ModelDataStorage.h"
#include "ModelObject.h"
#include "ShaderType.h"
#include "string"
#include <GameSettings.h>
#include <stdexcept>
#include <vector>

InventoryManager::InventoryManager()
{
	GameSharedDependencies::Set<InventoryManager>(this);

#ifdef WINDOWS_BUILD
	glm::vec3 gunPos = glm::vec3(0, 0.025, -0.2);
	glm::vec3 hamburgerPos = glm::vec3(0.027, -0.01, -0.2);
#endif
#ifdef Raspberry_BUILD
	glm::vec3 gunPos = glm::vec3(0, 0.035, -0.2);
	glm::vec3 hamburgerPos = glm::vec3(0.035, -0.01, -0.2);
#endif // Raspberry_BUILD

	m_itemDatas.push_back(
		new ItemData
		{
			Item::GUN,
			ModelDataStorage::INVENTORY_GUN,
			gunPos,
			glm::vec3(0.005)
		});

	m_itemDatas.push_back(
		new ItemData
		{
			Item::HAMBURGER,
			ModelDataStorage::HAMBURGER,
			hamburgerPos,
			glm::vec3(0.04)
		});

	ModelDataStorage* modelDataStorage = GameSharedDependencies::Get<ModelDataStorage>();
	Uknitty::Engine* engine = Uknitty::Engine::GetInstance();
	Uknitty::AssetManager* assetManager = engine->GetAssetManager();
	for(auto& itemData : m_itemDatas)
	{
		ModelDataStorage::ModelData* modelData = modelDataStorage->GetModelData(itemData->modelName);
		Uknitty::ModelObject* modelObject = engine->CreateGameObject<Uknitty::ModelObject>();
		modelObject->Initialize(assetManager->AutoGetModel(itemData->modelName, modelData->m_filePath), assetManager->AutoGetShaderProgram(Uknitty::ShaderType::UNLIT));
		modelObject->GetLocalTransform()->SetScale(itemData->scale);
		modelObject->GetLocalTransform()->SetPosition(itemData->position);
		modelObject->SetParent(engine->GetMainCamera());
		itemData->modelObject = modelObject;
	}

	HideInventory();
}

InventoryManager::~InventoryManager()
{
	for(auto& itemData : m_itemDatas)
	{
		delete itemData;
	}
	GameSharedDependencies::Remove<InventoryManager>();
}

void InventoryManager::Update(float deltaTime)
{
	for(auto& itemData : m_itemDatas)
	{
		if(!itemData->isUsed)
		{
			glm::vec3 rot = *itemData->modelObject->GetLocalTransform()->GetRotation();
			rot.y += 60 * deltaTime;
			itemData->modelObject->GetLocalTransform()->SetRotation(rot);
		}
	}
}

void InventoryManager::ShowInventory()
{
	for(auto& itemData : m_itemDatas)
	{
		if(!itemData->isUsed)
		{
			itemData->modelObject->EnableDrawSelf();
		}
	}
	m_isVisible = false;
}

void InventoryManager::HideInventory()
{
	for(auto& itemData : m_itemDatas)
	{
		if(!itemData->isUsed)
		{
			itemData->modelObject->DisableDrawSelf();
		}
	}
	m_isVisible = false;
}

void InventoryManager::SetCurrentItem(Item item)
{
	for(auto& itemData : m_itemDatas)
	{
		if(itemData->item == item)
		{
			if(!itemData->isUsed)
			{
				m_currentItem = item;
				return;
			}
		}
	}
	m_currentItem = Item::NONE;
}

void InventoryManager::UseCurrentItem()
{
	switch(m_currentItem)
	{
		case InventoryManager::Item::NONE:
			break;
		case InventoryManager::Item::GUN:
			GameSharedDependencies::Get<GameplayEvents>()->OnUsedGun();
			break;
		case InventoryManager::Item::HAMBURGER:
			for(auto& itemData : m_itemDatas)
			{
				if(itemData->item == m_currentItem)
				{
					itemData->isUsed = true;
				}
			}
			GameSharedDependencies::Get<GameplayEvents>()->OnUsedHamburger();
			m_currentItem = InventoryManager::Item::GUN;
			break;
		default:
			throw std::runtime_error("Invalid Item");
	}
}
