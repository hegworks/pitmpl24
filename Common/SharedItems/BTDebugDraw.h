#pragma once

#include "btBulletDynamicsCommon.h"
#include <glm/glm.hpp>
#include <vector>

namespace Uknitty
{

class ShaderProgram;
class CameraObject;

class BTDebugDraw : public btIDebugDraw
{
public:
	BTDebugDraw();
	~BTDebugDraw();
	virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override;
	virtual void setDebugMode(int debugMode) override { m_debugMode = debugMode; }
	virtual int getDebugMode() const override { return m_debugMode; }
	virtual void drawContactPoint([[maybe_unused]] const btVector3& PointOnB, [[maybe_unused]] const btVector3& normalOnB, [[maybe_unused]] btScalar [[maybe_unused]] distance, [[maybe_unused]] int lifeTime, [[maybe_unused]] const btVector3& color) {};
	virtual void reportErrorWarning([[maybe_unused]] const char* warningString) {};
	virtual void draw3dText([[maybe_unused]] const btVector3& location, [[maybe_unused]] const char* textString) {};
	virtual void flushLines(glm::mat4 cameraViewProjection);

private:
	int m_debugMode = 0;
	ShaderProgram* m_shaderProgram = nullptr;

	unsigned int m_vbo, m_vao, m_colorVbo;
	std::vector<float> m_vertexData;
	std::vector<float> m_colorData;
};

} // namespace Uknitty
