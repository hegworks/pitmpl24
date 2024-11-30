#include "Transform.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Transform::Transform()
{
	Reset();
}

Transform::~Transform()
{
}

void Transform::Reset()
{
	m_position = glm::vec3(0);
	m_rotation = glm::vec3(0);
	m_scale = glm::vec3(1);

	m_modelMatrix = glm::mat4(1);
}

void Transform::SetPosition(glm::vec3 pos)
{
	m_position = pos;
	CalculateModelMatrix();
}

void Transform::SetRotation(glm::vec3 rotation)
{
	m_rotation = rotation;
	CalculateModelMatrix();
}

void Transform::SetScale(glm::vec3 scale)
{
	m_scale = scale;
	CalculateModelMatrix();
}

void Transform::SetTransform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
{
	m_position = position;
	m_rotation = rotation;
	m_scale = scale;
	CalculateModelMatrix();
}

void Transform::CalculateModelMatrix()
{
	m_modelMatrix = glm::mat4(1);
	m_modelMatrix = glm::translate(m_modelMatrix, m_position);
	m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(m_rotation.x), glm::vec3(1, 0, 0));
	m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(m_rotation.y), glm::vec3(0, 1, 0));
	m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(m_rotation.z), glm::vec3(0, 0, 1));
	m_modelMatrix = glm::scale(m_modelMatrix, m_scale);
}
