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
	virtual void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) {};
	virtual void reportErrorWarning(const char* warningString) {};
	virtual void draw3dText(const btVector3& location, const char* textString) {};
	virtual void flushLines(glm::mat4 cameraViewProjection);

private:
	int m_debugMode = 0;
	ShaderProgram* m_shaderProgram = nullptr;

	unsigned int m_vbo, m_vao, m_colorVbo;
	std::vector<float> m_vertexData;
	std::vector<float> m_colorData;
};

} // namespace Uknitty
