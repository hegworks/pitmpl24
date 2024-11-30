#pragma once

#include <glm/glm.hpp>

namespace Uknitty
{

class Transform
{
public:
	Transform();
	~Transform();

	void Reset();
	void SetPosition(glm::vec3 pos);
	void SetRotation(glm::vec3 rotation);
	void SetScale(glm::vec3 scale);
	void SetTransform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

	glm::mat4* GetModelMatrix() { return &m_modelMatrix; }

	glm::vec3 m_position;
	glm::vec3 m_rotation;
	glm::vec3 m_scale;

private:
	void CalculateModelMatrix();

	glm::mat4 m_modelMatrix;
};

}