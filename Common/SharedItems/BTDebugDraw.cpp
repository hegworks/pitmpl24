#include "BTDebugDraw.h"

#include "btBulletDynamicsCommon.h"
#include "GlCheckError.h"
#include <ICamera.h>
#include <ShaderProgram.h>

#ifdef Raspberry_BUILD
#include <GLES3/gl3.h>
#endif

#ifdef WINDOWS_BUILD
#include "glad/glad.h"
#endif

namespace Uknitty
{

BTDebugDraw::BTDebugDraw(ICamera* camera)
{
	m_camera = camera;
	m_shaderProgram = new ShaderProgram("../Common/Assets/Shaders/BTDebugVertex.glsl", "../Common/Assets/Shaders/BTDebugFragment.glsl");
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
}

BTDebugDraw::~BTDebugDraw()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	m_shaderProgram->Delete();
}

void BTDebugDraw::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	std::vector<float> vertices =
	{
		from.getX(), from.getY(), from.getZ(),
		to.getX(), to.getY(), to.getZ()
	};

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	m_shaderProgram->Use();
	m_shaderProgram->SetMat4("uMVP", m_camera->GetProjection() * m_camera->GetView() * glm::mat4(1));
	m_shaderProgram->SetVec3("uColor", glm::vec3(color.getX(), color.getY(), color.getZ()));

	glBindVertexArray(VAO);
	glDrawArrays(GL_LINES, 0, 2);
}

void BTDebugDraw::CreateLine()
{

}

} // namespace Uknitty
