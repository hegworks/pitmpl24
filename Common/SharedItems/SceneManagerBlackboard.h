#pragma once

#include "glm/glm.hpp"

class SceneManagerBlackboard
{
#pragma region Scott Meyers Thread-Safe Singleton
public:
	static SceneManagerBlackboard* GetInstance()
	{
		static SceneManagerBlackboard instance;
		return &instance;
	}

	SceneManagerBlackboard(SceneManagerBlackboard const&) = delete;
	void operator=(SceneManagerBlackboard const&) = delete;

private:
	SceneManagerBlackboard() {}
#pragma endregion Scott Meyers Thread-Safe Singleton

public:
	void SetPlayerFeetPos(glm::vec3 pos) { m_playerFeetPos = pos; }
	const glm::vec3 GetPlayerFeetPos() const { return m_playerFeetPos; }

	void SetPlayerGunPos(glm::vec3 pos) { m_playerGunPos = pos; }
	const glm::vec3 GetPlayerGunPos() const { return m_playerGunPos; }

private:
	glm::vec3 m_playerFeetPos = glm::vec3(0);
	glm::vec3 m_playerGunPos = glm::vec3(0);
};
