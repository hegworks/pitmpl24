#include "Scene.h"

#include <iostream>
#include <stdexcept>
#include <tmxparser.h>

Scene::Scene(int mapId)
{
	std::string fullPath = MAPS_PATH + std::to_string(mapId) + MAPS_EXTENTION;
	tmxparser::TmxMap map;
	tmxparser::TmxReturn error = tmxparser::parseFromFile(fullPath, &map, MAPS_PATH);
	if(error)
	{
		throw std::runtime_error("parseFromFile failed for file: " + fullPath);
	}

	m_tmxMap = &map;
	std::cout << "YAY" << std::endl;
}

void Scene::ProcessMousePosition(double xPos, double yPos)
{
}

void Scene::ProcessKeyboard(IKeyboard* iKeyboard)
{
}

void Scene::KeyDown(Key key)
{
}

void Scene::KeyUp(Key key)
{
}

void Scene::Awake()
{
}

void Scene::Start()
{
}

void Scene::Update(float deltaTime)
{
}

void Scene::LateUpdate(float deltaTime)
{
}

void Scene::FixedUpdate()
{
}

void Scene::Destroy()
{
}

void Scene::Draw()
{
}
