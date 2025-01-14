#pragma once

#include "glm/glm.hpp"
#include "unordered_map"
#include <string>

class ModelDataStorage
{
public:
	ModelDataStorage();
	~ModelDataStorage();

	struct ModelData
	{
		std::string m_filePath;
		glm::vec3 m_dimensions;
	};

	ModelData* GetModelData(const std::string key);

	inline static constexpr const char* CRATE_2x4 = "Crate_2x4";
	inline static constexpr const char* CRATE_4x4 = "Crate_4x4";
	inline static constexpr const char* TANK = "Tank";
	inline static constexpr const char* FENCE = "Fence";
	inline static constexpr const char* WALL_1x4x1 = "Wall_1x4x1";
	inline static constexpr const char* WALL_2x4x1 = "Wall_2x4x1";
	inline static constexpr const char* WALL_1x4x2 = "Wall_1x4x2";
	inline static constexpr const char* EMPTY = "Empty";
	inline static constexpr const char* CUBE = "Cube";
	inline static constexpr const char* GROUND = "ground";
	inline static constexpr const char* PIKMIN = "pikmin";
	inline static constexpr const char* RETICLE = "Reticle";
	inline static constexpr const char* WIN = "Win";
	inline static constexpr const char* INVENTORY_GUN = "InventoryGun";

private:
	std::unordered_map<std::string, ModelData> m_datas;
};
