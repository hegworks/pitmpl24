#include "BTDebugDraw.h"

#include "btBulletDynamicsCommon.h"
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
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
}

BTDebugDraw::~BTDebugDraw()
{
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
	m_shaderProgram->Delete();
	delete m_shaderProgram;
}

// REF: this implementation has taken inspiration from https://stackoverflow.com/a/54069424
void BTDebugDraw::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	std::vector<float> vertices =
	{
		from.getX(), from.getY(), from.getZ(),
		to.getX(), to.getY(), to.getZ()
	};

	glBindVertexArray(m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	m_shaderProgram->Use();
	m_shaderProgram->SetMat4("uMVP", m_camera->GetProjection() * m_camera->GetView() * glm::mat4(1));
	m_shaderProgram->SetVec3("uColor", glm::vec3(color.getX(), color.getY(), color.getZ()));

	glBindVertexArray(m_vao);
	glDrawArrays(GL_LINES, 0, 2);
}

} // namespace Uknitty
