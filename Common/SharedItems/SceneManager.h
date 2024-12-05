#pragma once

#include "Interfaces.h"
#include <string>
#include <vector>

namespace Uknitty
{
class ShaderProgram;
}
class Scene;
class GeneralCamera;
class Player;

class SceneManager : public Uknitty::FlowInputRender
{
	// Inherited via FlowInputRender
	virtual void ProcessMousePosition(double xPos, double yPos) override;
	virtual void ProcessKeyboard(IKeyboard* iKeyboard) override;
	virtual void KeyDown(Key key) override;
	virtual void KeyUp(Key key) override;
	virtual void Awake() override;
	virtual void Start() override;
	virtual void Update(float deltaTime) override;
	virtual void LateUpdate(float deltaTime) override;
	virtual void FixedUpdate() override;
	virtual void Destroy() override;
	virtual void Draw() override;

private:
	Scene* m_currentScene = nullptr;
	GeneralCamera* m_camera = nullptr;
	Player* m_player = nullptr;
	Uknitty::ShaderProgram* m_shaderProgram = nullptr;

	std::vector<Uknitty::Input*> m_inputAbles;
	std::vector<Uknitty::Flow*> m_flowAbles;
	std::vector<Uknitty::Render*> m_renderAbles;
	std::vector<Uknitty::FlowInput*> m_flowInputAbles;
	std::vector<Uknitty::FlowInputRender*> m_flowInputRenderAbles;

	const int INITIAL_MAP_ID = 1;
	const std::string VERTEX_SHADER_PATH = "../Common/Assets/Maps/";
	const std::string FRAGMENT_SHADER_PATH = "../Common/Assets/Maps/";

	void LoadScene(int mapId);
	void CreatePlayer();
	void CreateCamera();
	void CreateShaderProgram();
};

