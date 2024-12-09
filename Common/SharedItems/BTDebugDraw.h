#pragma once

#include "btBulletDynamicsCommon.h"
#include <glm/glm.hpp>
#include <vector>

namespace Uknitty
{

class ShaderProgram;
class ICamera;

struct GLDebugLine
{
	int shaderProgramId;
	unsigned int VBO, VAO;
	std::vector<float> vertices;
	glm::vec3 startPoint;
	glm::vec3 endPoint;
	glm::mat4 MVP;
	glm::vec3 lineColor;
};

class BTDebugDraw : public btIDebugDraw
{
public:
	BTDebugDraw(ICamera* camera);
	~BTDebugDraw();
	virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override;
	virtual void setDebugMode(int debugMode) override { m_debugMode = debugMode; }
	virtual int getDebugMode() const override { return m_debugMode; }
	virtual void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) {};
	virtual void reportErrorWarning(const char* warningString) {};
	virtual void draw3dText(const btVector3& location, const char* textString) {};

private:
	int m_debugMode = 0;
	ICamera* m_camera = nullptr;
	ShaderProgram* m_shaderProgram = nullptr;

	unsigned int VBO, VAO;

	void CreateLine();
};

} // namespace Uknitty
