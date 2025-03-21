#pragma once

#include <glm/glm.hpp>

namespace Uknitty
{

class CTransform
{
public:
	CTransform();
	~CTransform() = default;

	void Reset();
	void SetPosition(glm::vec3 pos);
	/// rotation is in degrees
	void SetRotation(glm::vec3 rotation);
	void SetScale(glm::vec3 scale);
	void SetTransform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
	void OverrideMatrix(glm::mat4 mat);

	const glm::mat4* GetMatrix() const { return &m_matrix; }
	const glm::vec3* GetPosition() const { return &m_position; }
	const glm::vec3* GetRotation() const { return &m_rotation; }
	const glm::vec3* GetScale() const { return &m_scale; }
	glm::vec3 GetForward() const;
	glm::vec3 GetRight() const;

private:
	void CalculateModelMatrix();

	glm::mat4 m_matrix;
	glm::vec3 m_position;
	glm::vec3 m_rotation;
	glm::vec3 m_scale;
};

}