#pragma once

#include "glm/glm.hpp"

class SceneManagerBlackboard
{
public:
	void SetPlayerFeetPos(glm::vec3 pos) { m_playerFeetPos = pos; }
	const glm::vec3 GetPlayerFeetPos() const { return m_playerFeetPos; }

	void SetPlayerGunPos(glm::vec3 pos) { m_playerGunPos = pos; }
	const glm::vec3 GetPlayerGunPos() const { return m_playerGunPos; }

private:
	glm::vec3 m_playerFeetPos;
	glm::vec3 m_playerGunPos;
};
