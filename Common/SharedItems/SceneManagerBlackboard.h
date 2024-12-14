#pragma once

#include "glm/glm.hpp"

class SceneManagerBlackboard
{
public:
	void SetPlayerFeetPos(glm::vec3 pos) { m_playerFeetPos = pos; }
	const glm::vec3 GetPlayerFeetPos() const { return m_playerFeetPos; }

private:
	glm::vec3 m_playerFeetPos;
};
