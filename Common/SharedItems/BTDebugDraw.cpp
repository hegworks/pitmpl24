#include "BTDebugDraw.h"

#include "btBulletDynamicsCommon.h"
#include "SharedDependencies.h"
#include <CameraObject.h>
#include <ShaderProgram.h>

#ifdef Raspberry_BUILD
#include <GLES3/gl3.h>
#endif

#ifdef WINDOWS_BUILD
#include "glad/glad.h"
#endif

namespace Uknitty
{

BTDebugDraw::BTDebugDraw()
{
	m_camera = SharedDependencies::GetCamera();
	m_shaderProgram = new ShaderProgram("../Common/Assets/Shaders/BTDebugVertex.glsl", "../Common/Assets/Shaders/BTDebugFragment.glsl");
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_colorVbo);
}

BTDebugDraw::~BTDebugDraw()
{
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_colorVbo);
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

	m_vertexData.insert(m_vertexData.end(), vertices.begin(), vertices.end());

	for(int i = 0; i < 2; ++i)
	{
		m_colorData.insert(
			m_colorData.end(),
			{
				color.getX(), color.getY(), color.getZ()
			});
	}
}

void BTDebugDraw::flushLines()
{
	if(m_vertexData.empty()) return;

	glBindVertexArray(m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, m_vertexData.size() * sizeof(float), m_vertexData.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, m_colorVbo);
	glBufferData(GL_ARRAY_BUFFER, m_colorData.size() * sizeof(float), m_colorData.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);

#if 0
	m_shaderProgram->Use();
	m_shaderProgram->SetMat4("uMVP", m_camera->GetProjection() * m_camera->GetView() /* *glm::mat4(1) */);

	glDrawArrays(GL_LINES, 0, m_vertexData.size() / 3);
#endif

	glBindVertexArray(0);

	m_vertexData.clear();
	m_colorData.clear();
}

} // namespace Uknitty
