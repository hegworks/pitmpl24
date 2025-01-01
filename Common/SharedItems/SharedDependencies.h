#pragma once

#include "btBulletDynamicsCommon.h"
#include <stdexcept>

namespace Uknitty
{
class CameraObject;
class ShaderProgram;
}

class SharedDependencies
{
public:
	SharedDependencies() = default;
	~SharedDependencies() = default;

#pragma region Setters
	const static void SetDynamicsWorld(btDiscreteDynamicsWorld* btDynamicsWorld)
	{
		if(!btDynamicsWorld) throw std::runtime_error("btDynamicsWorld is nullptr");
		m_btDynamicsWorld = btDynamicsWorld;
	}
	const static void SetCamera(Uknitty::CameraObject* camera)
	{
		if(!camera) throw std::runtime_error("Camera is nullptr");
		m_camera = camera;
	}
	const static void SetShaderProgram(Uknitty::ShaderProgram* shaderProgram)
	{
		if(!shaderProgram) throw std::runtime_error("ShaderProgram is nullptr");
		m_shaderProgram = shaderProgram;
	}
#pragma endregion Setters

#pragma region Getters
	static btDiscreteDynamicsWorld* GetDynamicsWorld()
	{
		if(!m_btDynamicsWorld) throw std::runtime_error("DynamicsWorld has not been set, but something is trying to access it");
		return const_cast<btDiscreteDynamicsWorld*>(m_btDynamicsWorld);
	}
	static Uknitty::CameraObject* GetCamera()
	{
		if(!m_camera) throw std::runtime_error("Camera has not been set, but something is trying to access it");
		return const_cast<Uknitty::CameraObject*>(m_camera);
	}
	static Uknitty::ShaderProgram* GetShaderProgram()
	{
		if(!m_shaderProgram) throw std::runtime_error("ShaderProgram has not been set, but something is trying to access it");
		return const_cast<Uknitty::ShaderProgram*>(m_shaderProgram);
	}
#pragma endregion Getters

private:
	inline static const btDiscreteDynamicsWorld* m_btDynamicsWorld = nullptr;
	inline static const Uknitty::CameraObject* m_camera = nullptr;
	inline static const Uknitty::ShaderProgram* m_shaderProgram = nullptr;
};
