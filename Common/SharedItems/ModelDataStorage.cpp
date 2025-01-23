#include "ModelDataStorage.h"

#include "GameSettings.h"
#include "GameSharedDependencies.h"
#include "glm/glm.hpp"
#include <stdexcept>
#include <string>

ModelDataStorage::ModelDataStorage()
{
	GameSharedDependencies::Set<ModelDataStorage>(this);

	m_datas[CRATE_2x4] =
	{
		"../Common/Assets/Models/Crate_2x4/Crate.obj",
		glm::vec3(2, 1, 4)
	};
	m_datas[CRATE_4x4] =
	{
		"../Common/Assets/Models/Crate_4x4/Crate.obj",
		glm::vec3(4, 1, 4)
	};
	m_datas[TANK] =
	{
		"../Common/Assets/Models/Tank/Tank.obj",
		glm::vec3(6, 3, 7)
	};
	m_datas[FENCE] =
	{
		"../Common/Assets/Models/Fence/Fence2.obj",
		glm::vec3(4, 4, 0.3)
	};
	m_datas[WALL_1x4x1] =
	{
		"../Common/Assets/Models/Wall_1x4x1/Wall_1x4x1.obj",
		glm::vec3(1, 4, 1)
	};
	m_datas[WALL_2x4x1] =
	{
		"../Common/Assets/Models/Wall_2x4x1/Wall_2x4x1.obj",
		glm::vec3(2, 4, 1)
	};
	m_datas[WALL_1x4x2] =
	{
		"../Common/Assets/Models/Wall_1x4x2/Wall_1x4x2.obj",
		glm::vec3(1, 4, 2)
	};
	m_datas[EMPTY] =
	{
		"../Common/Assets/Models/Empty/Empty.obj",
		glm::vec3(1, 1, 1)
	};
	m_datas[CUBE] =
	{
		"../Common/Assets/Models/Primitives/Cube/Cube.obj",
		glm::vec3(1, 1, 1)
	};
	m_datas[SPHERE] =
	{
		"../Common/Assets/Models/Primitives/Sphere/Sphere.obj",
		glm::vec3(1, 1, 1)
	};
	m_datas[GROUND] =
	{
		"../Common/Assets/Models/Ground/Plane.obj",
		glm::vec3(MAP_SCALE_X, 1, MAP_SCALE_Z)
	};
	m_datas[PIKMIN] =
	{
		"../Common/Assets/Models/Pikmin/Pikmin.obj",
		glm::vec3(1, 1, 1)
	};
	m_datas[RETICLE] =
	{
		"../Common/Assets/Models/Reticle/Sphere.obj",
		glm::vec3(1, 1, 1)
	};
	m_datas[WIN] =
	{
		"../Common/Assets/Models/Win/Win.obj",
		glm::vec3(4, 4, 4)
	};
	m_datas[INVENTORY_GUN] =
	{
		"../Common/Assets/Models/InventoryGun/Rifle_01.fbx",
		glm::vec3(1, 1, 1)
	};
	m_datas[HAMBURGER] =
	{
		"../Common/Assets/Models/Hamburger/Hamburger.fbx",
		glm::vec3(1, 1, 1)
	};
	m_datas[POINT_LIGHT] =
	{
		"../Common/Assets/Models/PointLightSource/Sphere.obj",
		glm::vec3(1, 1, 1)
	};
}

ModelDataStorage::~ModelDataStorage()
{
	m_datas.clear();
}

ModelDataStorage::ModelData* ModelDataStorage::GetModelData(const std::string key)
{
	if(m_datas.find(key) == m_datas.end())
	{
		throw std::runtime_error("ModelData not found for key: " + key);
	}
	return &m_datas[key];
}
