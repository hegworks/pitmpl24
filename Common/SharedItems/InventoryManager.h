#pragma once

#include "glm/glm.hpp"
#include "ModelDataStorage.h"
#include "string"
#include <vector>

namespace Uknitty
{
class ModelObject;
}

class InventoryManager
{
public:
	InventoryManager();
	~InventoryManager();

	enum class Item
	{
		NONE,
		GUN,
		HAMBURGER,
	};

	struct ItemData
	{
		Item item;
		std::string modelName;
		glm::vec3 position;
		glm::vec3 scale;
		Uknitty::ModelObject* modelObject = nullptr;
		bool isUsed = false;
	};

	void Update(float deltaTime);
	void ShowInventory();
	void HideInventory();
	Item GetCurrentItem() const { return m_currentItem; }
	void SetCurrentItem(Item item);
	void UseCurrentItem();

private:
	std::vector<ItemData*> m_itemDatas;

	bool m_isVisible = false;
	Item m_currentItem = Item::GUN;
};
