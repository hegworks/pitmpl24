#include "CTransform.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Uknitty
{

CTransform::CTransform()
{
	Reset();
}

CTransform::~CTransform()
{
}

void CTransform::Reset()
{
	m_position = glm::vec3(0);
	m_rotation = glm::vec3(0);
	m_scale = glm::vec3(1);

	m_matrix = glm::mat4(1);
}

void CTransform::SetPosition(glm::vec3 pos)
{
	m_position = pos;
	CalculateModelMatrix();
}

void CTransform::SetRotation(glm::vec3 rotation)
{
	m_rotation = rotation;
	CalculateModelMatrix();
}

void CTransform::SetScale(glm::vec3 scale)
{
	m_scale = scale;
	CalculateModelMatrix();
}

void CTransform::SetTransform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
{
	m_position = position;
	m_rotation = rotation;
	m_scale = scale;
	CalculateModelMatrix();
}

glm::vec3 CTransform::GetForward() const
{
	const glm::mat4 invertedModelMat = glm::inverse(m_matrix);
	const glm::vec3 forward = glm::normalize(glm::vec3(invertedModelMat[2]) * glm::vec3(-1, -1, 1));
	return forward;
}

glm::vec3 CTransform::GetRight() const
{
	glm::vec3 forward = GetForward();
	glm::vec3 up = glm::vec3(0, 1, 0);
	return glm::normalize(glm::cross(forward, up));
}

void CTransform::CalculateModelMatrix()
{
	m_matrix = glm::mat4(1);
	m_matrix = glm::translate(m_matrix, m_position);
	m_matrix = glm::rotate(m_matrix, glm::radians(m_rotation.x), glm::vec3(1, 0, 0));
	m_matrix = glm::rotate(m_matrix, glm::radians(m_rotation.y), glm::vec3(0, 1, 0));
	m_matrix = glm::rotate(m_matrix, glm::radians(m_rotation.z), glm::vec3(0, 0, 1));
	m_matrix = glm::scale(m_matrix, m_scale);
}

} // namespace Uknitty
