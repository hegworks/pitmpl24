#include "Game.h"

#include "Common.h"
#include "FreeFlyCamera.h"
#include "GeneralCamera.h"
#include "ICamera.h"
#include "IGraphics.h"
#include "IInput.h"
#include "Interfaces.h"
#include "Player.h"
#include "SceneManager.h"
#include "SharedInput.h"
#include "SolidObject.h"
#include "Transform.h"
#include <Model.h>
#include <ShaderProgram.h>

#include "ImGui-master/backends/imgui_impl_opengl3.h"
#include "ImGui-master/imgui.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "tmxparser.h"
#include "TmxParserHelper.h"

#include <chrono>
#include <string>

#ifdef Raspberry_BUILD
#include <GLES3/gl3.h>
#endif

#ifdef WINDOWS_BUILD
#include "glad/glad.h"

#if defined(DEBUG) && defined(VISUAL_LEAK_DETECTOR)
#include <vld.h>
#endif // DEBUG

#endif

Game::Game(SharedInput* sharedInput, IGraphics* iGraphics) :
	m_sharedInput(sharedInput),
	m_iGraphics(iGraphics)

{
	m_iMouse = m_sharedInput->GetMouse();
	m_iKeyboard = m_sharedInput->GetKeyboard();

	m_sharedInput->GetKeyboard()->SetKeyCallback(
		[this](Key key, KeyAction action) { KeyCallback(key, action); }
	);
}

Game::~Game()
{

}

enum class WallType
{
	VERTICAL,
	HORIZONTAL,
	UNIFORM,
};

struct WallData
{
	glm::vec2 position;
	glm::ivec2 size;
	WallType wallType;
};

template <typename T>
void ProcessInputCaller(std::vector<T*>& inputAbles, IMouse* mouse, IKeyboard* keyboard)
{
	for(T* i : inputAbles)
	{
		i->ProcessMousePosition(mouse->GetPosition().x, mouse->GetPosition().y);
		i->ProcessKeyboard(keyboard);
	}
}

template <typename T>
void AwakeCaller(std::vector<T*>& flowAble)
{
	for(T* i : flowAble)
	{
		i->Awake();
	}
}

template <typename T>
void StartCaller(std::vector<T*>& flowAble)
{
	for(T* i : flowAble)
	{
		i->Start();
	}
}

template <typename T>
void UpdateCaller(std::vector<T*>& flowAble, float deltaTime)
{
	for(T* i : flowAble)
	{
		i->Update(deltaTime);
	}
}

template <typename T>
void LateUpdateCaller(std::vector<T*>& flowAble, float deltaTime)
{
	for(T* i : flowAble)
	{
		i->LateUpdate(deltaTime);
	}
}

template <typename T>
void DrawCaller(std::vector<T*>& renderAble)
{
	for(T* i : renderAble)
	{
		i->Draw();
	}
}

template <typename T>
void KeyDownCaller(std::vector<T*>& inputAble, Key key)
{
	for(T* i : inputAble)
	{
		i->KeyDown(key);
	}
}

template <typename T>
void KeyUpCaller(std::vector<T*>& inputAble, Key key)
{
	for(T* i : inputAble)
	{
		i->KeyUp(key);
	}
}

void Game::Start()
{
	InitializeOpenGLES();

	printf("This cross project was partly inspired by BUas Student Ferri de Lange\n");
	printf("This GPU supplied by  :%s\n", glGetString(GL_VENDOR));
	printf("This GPU supports GL  :%s\n", glGetString(GL_VERSION));
	printf("This GPU Renders with :%s\n", glGetString(GL_RENDERER));
	printf("This GPU Shaders are  :%s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

#pragma region imgui
	// imgui setup setup for ES3.x, use #verion 100 for GLES2.0 
	const char* glsl_version = "#version 100";  //310/320es not available
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplOpenGL3_Init(glsl_version);
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2(SCRWIDTH, SCRHEIGHT);
#pragma endregion imgui

#pragma region Other Initializations
	m_sceneManager = new SceneManager();
	flowInputRenderAbles.push_back(m_sceneManager);

	/*
#pragma region tmxparser
	const int TILE_SIZE = 32;
	const glm::vec2 MAP_CENTER = glm::vec2(16, 12);
	const std::string CRATE_2_X_4_OBJECTGROUP = "crate2x4";
	const std::string CRATE_4_X_4_OBJECTGROUP = "crate4x4";
	const std::string TANK_OBJECTGROUP = "tank";
	const std::string WALL_OBJECTGROUP = "wall";

	std::vector<glm::ivec2> crate2x4positions;
	std::vector<glm::ivec2> crate4x4positions;
	std::vector<glm::ivec2> tankPositions;
	std::vector<WallData*> wallDatas;

	tmxparser::TmxReturn error;
	tmxparser::TmxMap map;

	// test from file
	error = tmxparser::parseFromFile("../Common/Assets/Maps/1.tmx", &map, "../Common/Assets/Maps/");

	if(!error)
	{
		std::vector<tmxparser::TmxObjectGroup> objectGroups = (&map)->objectGroupCollection;

		for(tmxparser::TmxObjectGroup& objectGroup : objectGroups)
		{
			if(objectGroup.name == CRATE_2_X_4_OBJECTGROUP)
			{
				for(tmxparser::TmxObject& object : objectGroup.objects)
				{
					crate2x4positions.push_back(MAP_CENTER - glm::vec2(object.x / TILE_SIZE, object.y / TILE_SIZE));
				}
			}
			if(objectGroup.name == CRATE_4_X_4_OBJECTGROUP)
			{
				for(tmxparser::TmxObject& object : objectGroup.objects)
				{
					crate4x4positions.push_back(MAP_CENTER - glm::vec2(object.x / TILE_SIZE, object.y / TILE_SIZE));
				}
			}
			if(objectGroup.name == TANK_OBJECTGROUP)
			{
				for(tmxparser::TmxObject& object : objectGroup.objects)
				{
					tankPositions.push_back(MAP_CENTER - glm::vec2(object.x / TILE_SIZE, object.y / TILE_SIZE));
				}
			}
			if(objectGroup.name == WALL_OBJECTGROUP)
			{
				for(tmxparser::TmxObject& object : objectGroup.objects)
				{
					WallData* wallData = new WallData();
					wallData->position =
						MAP_CENTER
						- glm::vec2((object.x + (object.width / 2.0f)) / TILE_SIZE, (object.y + (object.height / 2.0f)) / TILE_SIZE);
					wallData->size = glm::ivec2(object.width / TILE_SIZE, object.height / TILE_SIZE);
					wallData->wallType = static_cast<WallType>(std::stoi(object.propertyMap["vhu"]));
					wallDatas.push_back(wallData);
				}
			}
		}
	}
	else
	{
		printf("error parsing file");
	}


	Uknitty::Model* crate2x4Model = new Uknitty::Model("../Common/Assets/Models/Crate_2x4/Crate.obj");
	for(auto& crate2x4pos : crate2x4positions)
	{
		SolidObject* crate2x4Object = new SolidObject(m_iCamera, crate2x4Model, shaderProgram);
		crate2x4Object->GetTransform()->SetPosition(glm::vec3(crate2x4pos.x, 0, crate2x4pos.y));
		renderAbles.push_back(crate2x4Object);
	}

	Uknitty::Model* crate4x4Model = new Uknitty::Model("../Common/Assets/Models/Crate_4x4/Crate.obj");
	for(auto& crate4x4pos : crate4x4positions)
	{
		SolidObject* crate4x4Object = new SolidObject(m_iCamera, crate4x4Model, shaderProgram);
		crate4x4Object->GetTransform()->SetPosition(glm::vec3(crate4x4pos.x, 0, crate4x4pos.y));
		renderAbles.push_back(crate4x4Object);
	}

	Uknitty::Model* tankModel = new Uknitty::Model("../Common/Assets/Models/Tank/Tank.obj");
	for(auto& tankPos : tankPositions)
	{
		SolidObject* tankObject = new SolidObject(m_iCamera, tankModel, shaderProgram);
		tankObject->GetTransform()->SetPosition(glm::vec3(tankPos.x, 0, tankPos.y));
		renderAbles.push_back(tankObject);
	}

	for(auto& wallData : wallDatas)
	{
		switch(wallData->wallType)
		{
			case WallType::VERTICAL:
			{
				glm::vec3 wallVerticalScale = glm::vec3(wallData->size.x, 1, wallData->size.y);

				Uknitty::Model* wallModel = nullptr;
				if(wallData->size.x == 1)
				{
					wallModel = new Uknitty::Model("../Common/Assets/Models/Wall_1x4x1/Wall_1x4x1.obj", glm::vec2(wallVerticalScale.z, 1));
				}
				else if(wallData->size.x == 2)
				{
					wallModel = new Uknitty::Model("../Common/Assets/Models/Wall_2x4x1/Wall_2x4x1.obj", glm::vec2(wallVerticalScale.z, 1));
				}
				else
				{
					throw std::runtime_error("vertical wall width bigger than 2 is not supported yet");
				}

				SolidObject* wallVerticalObject = new SolidObject(m_iCamera, wallModel, shaderProgram);
				wallVerticalObject->GetTransform()->SetScale(glm::vec3(1, 1, wallVerticalScale.z)); // x scale is built in the loaded wallModel
				wallVerticalObject->GetTransform()->SetPosition(glm::vec3(wallData->position.x, 0, wallData->position.y));
				renderAbles.push_back(wallVerticalObject);
			}

			break;

			case WallType::HORIZONTAL:
			{
				glm::vec3 wallHorizontalScale = glm::vec3(wallData->size.x, 1, wallData->size.y);

				Uknitty::Model* wallModel = nullptr;
				if(wallData->size.y == 1)
				{
					wallModel = new Uknitty::Model("../Common/Assets/Models/Wall_1x4x1/Wall_1x4x1.obj", glm::vec2(wallHorizontalScale.x, 1));
				}
				else if(wallData->size.y == 2)
				{
					wallModel = new Uknitty::Model("../Common/Assets/Models/Wall_1x4x2/Wall_1x4x2.obj", glm::vec2(wallHorizontalScale.x, 1));
				}
				else
				{
					throw std::runtime_error("horizontal wall width bigger than 2 is not supported yet");
				}

				SolidObject* wallHorizontalObject = new SolidObject(m_iCamera, wallModel, shaderProgram);
				wallHorizontalObject->GetTransform()->SetScale(glm::vec3(wallHorizontalScale.x, 1, 1)); // z scale is built in the loaded wallModel
				wallHorizontalObject->GetTransform()->SetPosition(glm::vec3(wallData->position.x, 0, wallData->position.y));
				renderAbles.push_back(wallHorizontalObject);
			}

			break;

			case WallType::UNIFORM:
			{
				glm::vec3 wallUniformScale = glm::vec3(wallData->size.x, 1, wallData->size.y);
				Uknitty::Model* wallUniformModel = new Uknitty::Model("../Common/Assets/Models/Wall_1x4x1/Wall_1x4x1.obj", glm::vec2(wallUniformScale.x, 1));
				SolidObject* wallUnifromObject = new SolidObject(m_iCamera, wallUniformModel, shaderProgram);
				wallUnifromObject->GetTransform()->SetScale(wallUniformScale);
				wallUnifromObject->GetTransform()->SetPosition(glm::vec3(wallData->position.x, 0, wallData->position.y));
				renderAbles.push_back(wallUnifromObject);
			}

			break;

			default:
				throw std::runtime_error("uknown WallType");
		}
	}

#pragma endregion tmxparser

	Uknitty::Model* snake = new Uknitty::Model("../Common/Assets/Models/NakedSnake/NakedSnake.obj");
	m_player = new Player(snake, m_iCamera, shaderProgram);
	flowInputRenderAbles.push_back(m_player);

	static_cast<GeneralCamera*>(m_iCamera)->SetFollowTransform(m_player->m_transform);

	Uknitty::Model* worldCenter = new Uknitty::Model("../Common/Assets/Models/Primitives/Cube/Cube.obj");
	SolidObject* worldCenterObject = new SolidObject(m_iCamera, worldCenter, shaderProgram);
	worldCenterObject->GetTransform()->SetScale(glm::vec3(0.05, 100, 0.05));
	renderAbles.push_back(worldCenterObject);

	Uknitty::Model* plane = new Uknitty::Model("../Common/Assets/Models/Primitives/Plane/Plane.obj", glm::vec2(24, 32));
	SolidObject* planeObject = new SolidObject(m_iCamera, plane, shaderProgram);
	planeObject->GetTransform()->SetScale(glm::vec3(32, 0, 24));
	renderAbles.push_back(planeObject);
	*/

	AwakeCaller(flowAbles);
	AwakeCaller(flowInputAbles);
	AwakeCaller(flowInputRenderAbles);

	StartCaller(flowAbles);
	StartCaller(flowInputAbles);
	StartCaller(flowInputRenderAbles);

#pragma endregion Other Initializations

#pragma region Timing
	auto startTime = std::chrono::system_clock::now();
	auto lastTime = startTime;

	static int frames = 0;
	static double starttime = 0;
	static bool first = true;
	static int fps = 0;

#pragma endregion Timing

	float degree = 0;

	while(!quitting)
	{
#pragma region Timing
		auto time = std::chrono::system_clock::now();
		std::chrono::duration<float> delta = time - lastTime;
		gameDeltaTime = delta.count() * 10.0f;

		std::chrono::duration<float> elapsed = time - startTime;
		if(first)
		{
			frames = 0;
			starttime = elapsed.count();
			first = false;
		}
		else
		{
			frames++;
			if(elapsed.count() - starttime > 0.25 && frames > 10)
			{
				fps = static_cast<int>(frames / (elapsed.count() - starttime));
				starttime = elapsed.count();
				frames = 0;
			}
		}
#pragma endregion Timing

		ProcessInputCaller(inputAbles, m_iMouse, m_iKeyboard);
		ProcessInputCaller(flowInputAbles, m_iMouse, m_iKeyboard);
		ProcessInputCaller(flowInputRenderAbles, m_iMouse, m_iKeyboard);

		UpdateCaller(flowAbles, gameDeltaTime);
		UpdateCaller(flowInputAbles, gameDeltaTime);
		UpdateCaller(flowInputRenderAbles, gameDeltaTime);

		LateUpdateCaller(flowAbles, gameDeltaTime);
		LateUpdateCaller(flowInputAbles, gameDeltaTime);
		LateUpdateCaller(flowInputRenderAbles, gameDeltaTime);

		ClearScreen();
		glViewport(0, 0, SCRWIDTH, SCRHEIGHT);

		DrawCaller(renderAbles);
		DrawCaller(flowInputRenderAbles);


#pragma region imgui
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();
		ImGui::SetNextWindowBgAlpha(0.2f);
		ImGui::SetNextWindowPos(ImVec2(10, 100));
		ImGuiWindowFlags window_flags = /*ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar  | */ ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoScrollbar;
		ImGui::SetNextWindowSize(ImVec2(150, 50));

		static bool open = true;
		// open a new window
		ImGui::Begin("AVG FPS", &open, window_flags);

		// this can be anything
		ImGui::Text(std::to_string((int)fps).c_str());

		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		// ImGui::EndFrame(); // actuall this is closed by the render
#pragma endregion imgui

		glFlush();
		m_iGraphics->SwapBuffer();
		lastTime = time;
		++frameCount;
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();
	m_iGraphics->Quit();
}

#pragma region Input
void Game::KeyCallback(Key key, KeyAction action)
{
	if(key == Key::ESCAPE && action == KeyAction::DOWN) Quit();

	if(action == KeyAction::DOWN)
	{
		KeyDownCaller(inputAbles, key);
		KeyDownCaller(flowInputAbles, key);
		KeyDownCaller(flowInputRenderAbles, key);
	}
	else if(action == KeyAction::UP)
	{
		KeyUpCaller(inputAbles, key);
		KeyUpCaller(flowInputAbles, key);
		KeyUpCaller(flowInputRenderAbles, key);
	}
}
#pragma endregion Input

void Game::InitializeOpenGLES()
{
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glDepthRangef(0.0f, 1.0f);
	glClearDepthf(1.0f);

	glDisable(GL_BLEND);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);

	glCullFace(GL_BACK);
	glViewport(0, 0, SCRWIDTH, SCRHEIGHT);
}

void Game::ClearScreen()
{
	glClearColor(189.0f / 256.0f, 224 / 256.0f, 254 / 256.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Game::Quit()
{
	quitting = true;
}
