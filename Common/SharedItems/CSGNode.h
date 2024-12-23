#pragma once

#include "glm/glm.hpp"
#include <vector>

namespace Uknitty
{
#if 0
class GameObject;

class CSGNode
{
public:
	CSGNode(GameObject* owner);
	void Draw(glm::mat4 parentsMVP);

private:
	GameObject* m_ownerGameObject = nullptr;
	CSGNode* m_parent = nullptr;
	std::vector<CSGNode*> m_children;
};
#endif
} // namespace Uknitty