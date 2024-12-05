#pragma once

#include "Interfaces.h"
#include "tmxparser.h"
#include <string>
#include <vector>

class GeneralCamera;
class Player;

class Scene : Uknitty::FlowInputRender
{
public:
	Scene(int mapId);

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
	GeneralCamera* m_generalCamera = nullptr;
	Player* m_player = nullptr;
	tmxparser::TmxMap* m_tmxMap = nullptr;

	std::vector<Uknitty::Input*> m_inputAbles;
	std::vector<Uknitty::Flow*> m_flowAbles;
	std::vector<Uknitty::Render*> m_renderAbles;
	std::vector<Uknitty::FlowInput*> m_flowInputAbles;
	std::vector<Uknitty::FlowInputRender*> m_flowInputRenderAbles;

	const std::string MAPS_PATH = "../Common/Assets/Maps/";
	const std::string MAPS_EXTENTION = ".tmx";
};

