#pragma once

#include "glm/glm.hpp"
#include "Interfaces.h"
#include <memory>
#include <vector>

namespace Uknitty
{

class Transform;
class Model;

class SceneGraphNode : Uknitty::GameObject
{
public:
	void Draw();
	void AddChild(SceneGraphNode* child)
	{
		m_children.push_back(child);
		child->SetParent(this);
	}
	void RemoveChild(SceneGraphNode* child)
	{
		m_children.erase(std::remove(m_children.begin(), m_children.end(), child), m_children.end());

	}

	void SetParent(SceneGraphNode* parent) { m_parent = parent; }
	void SetModel(Model* model) { m_model = model; }
	void SetLocalMat(glm::mat4 mat) { m_localTransformMat = mat; }
	bool HasChildren() { return m_children.size() > 0; }
	bool HasParent() { return m_parent != nullptr; }

	glm::mat4* GetLocalTransformMat() { return &m_localTransformMat; }
	glm::mat4* GetCalculatedTransformMat() { return &m_calculatedTransformMat; }

public:
	glm::mat4 m_localTransformMat = glm::mat4(1);
	glm::mat4 m_calculatedTransformMat = glm::mat4(1);
	Model* m_model = nullptr;

	SceneGraphNode* m_parent = nullptr;
	std::vector<SceneGraphNode*> m_children;
};

} // namespace Uknitty
