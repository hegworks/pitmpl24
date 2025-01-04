#pragma once

#include <stdexcept>

class ModelDataStorage;
class Player;

class GameSharedDependencies
{
public:
	GameSharedDependencies() = default;
	~GameSharedDependencies() = default;

#pragma region Setters
	const static void SetModelDataStorage(ModelDataStorage* modelDataStorage)
	{
		if(!modelDataStorage) throw std::runtime_error("modelDataStorage is nullptr");
		m_modelDataStorage = modelDataStorage;
	}
	const static void SetPlayer(Player* player)
	{
		if(!player) throw std::runtime_error("player is nullptr");
		m_player = player;
	}
#pragma endregion Setters

#pragma region Getters
	static ModelDataStorage* GetModelDataStorage()
	{
		if(!m_modelDataStorage) throw std::runtime_error("ModelDataStorage has not been set, but something is trying to access it");
		return const_cast<ModelDataStorage*>(m_modelDataStorage);
	}
	static Player* GetPlayer()
	{
		if(!m_player) throw std::runtime_error("Player has not been set, but something is trying to access it");
		return const_cast<Player*>(m_player);
	}
#pragma endregion Getters

private:
	inline static const ModelDataStorage* m_modelDataStorage = nullptr;
	inline static const Player* m_player = nullptr;
};
