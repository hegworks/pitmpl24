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

	constexpr static const char* CRATE_2x4 = "Crate_2x4";
	constexpr static const char* CRATE_4x4 = "Crate_4x4";
	constexpr static const char* TANK = "Tank";
	constexpr static const char* FENCE = "Fence";
	constexpr static const char* WALL_1x4x1 = "Wall_1x4x1";
	constexpr static const char* WALL_2x4x1 = "Wall_2x4x1";
	constexpr static const char* WALL_1x4x2 = "Wall_1x4x2";
	constexpr static const char* EMPTY = "Empty";
	constexpr static const char* CUBE = "Cube";
	constexpr static const char* GROUND = "ground";
	constexpr static const char* PIKMIN = "pikmin";
	constexpr static const char* RETICLE = "Reticle";

private:
	std::unordered_map<std::string, ModelData> m_datas;
};
