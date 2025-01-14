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
		GUN,
		HAMBURGER,
	};

	void Update(float deltaTime);
	void ShowInventory();
	void HideInventory();
	Item GetCurrentItem() const { return m_currentItem; }
	void SetCurrentItem(Item item) { m_currentItem = item; }

private:
	std::vector<std::string> m_modelObjectNames = {ModelDataStorage::INVENTORY_GUN, ModelDataStorage::HAMBURGER};
	std::vector<Uknitty::ModelObject*> m_modelObjects;
	std::vector<glm::vec3> m_positions = {glm::vec3(0, 0.04, -0.2),glm::vec3(0.045, -0.011, -0.2)};
	std::vector<glm::vec3> m_scales = {glm::vec3(0.01),glm::vec3(0.05)};

	bool m_isVisible = false;
	Item m_currentItem = Item::GUN;
};
