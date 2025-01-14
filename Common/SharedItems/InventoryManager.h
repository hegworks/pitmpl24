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

	void Update(float deltaTime);
	void ShowInventory();
	void HideInventory();

private:
	std::vector<std::string> m_modelObjectNames = {ModelDataStorage::INVENTORY_GUN};
	std::vector<Uknitty::ModelObject*> m_modelObjects;
	std::vector<glm::vec3> m_positions;

	bool m_isVisible = false;
};
