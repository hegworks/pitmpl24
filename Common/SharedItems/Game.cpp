#include "Game.h"

#include "AssetManager.h"
#include "CameraObject.h"
#include "CPhysics.h"
#include "CTransform.h"
#include "DynamicObject.h"
#include "Engine.h"
#include "GameManager.h"
#include "GameSettings.h"
#include "GameSharedDependencies.h"
#include "GeneralCamera.h"
#include "IGraphics.h"
#include "IInput.h"
#include "Model.h"
#include "ModelObject.h"
#include "PhysicsCollisionFilters.h"
#include "Player.h"
#include "SceneManager.h"
#include "ShaderProgram.h"
#include "SharedInput.h"
#include "StaticObject.h"
#include "UknittySettings.h"

#include "ImGui-master/backends/imgui_impl_opengl3.h"
#include "ImGui-master/imgui.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <chrono>
#include <string>

#ifdef Raspberry_BUILD
#include <GLES3/gl3.h>
#endif

#ifdef WINDOWS_BUILD
#include "glad/glad.h"

#if defined(_DEBUG) && defined(VISUAL_LEAK_DETECTOR)
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
	io.DisplaySize = ImVec2(Uknitty::SCRWIDTH, Uknitty::SCRHEIGHT);
#pragma endregion imgui

#pragma region Other Initializations
	m_gameManager = new GameManager(m_iMouse, m_iKeyboard);
#pragma endregion Other Initializations

#pragma region Timing
	auto startTime = std::chrono::system_clock::now();
	auto lastTime = startTime;

	static int frames = 0;
	static double starttime = 0;
	static bool first = true;
	static int fps = 0;

#pragma endregion Timing

	while(!quitting)
	{
#pragma region Timing
		auto time = std::chrono::system_clock::now();
		std::chrono::duration<float> delta = time - lastTime;
		gameDeltaTime = delta.count();

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

		ClearScreen();
		glViewport(0, 0, Uknitty::SCRWIDTH, Uknitty::SCRHEIGHT);

		ProcessMouse();
		m_gameManager->Update(gameDeltaTime);
		if(m_gameManager->ShouldQuit())
		{
			quitting = true;
		}

#pragma region imgui
		if(!m_iMouse->IsCapturingMouseInput())
		{
			io.MousePos.x = m_iMouse->GetPosition().x;
			io.MousePos.y = m_iMouse->GetPosition().y;
		}

#if 0
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

		ImGui::ShowDemoWindow();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif

		// ImGui::EndFrame(); // actuall this is closed by the render
#pragma endregion imgui

		glFlush();
		m_iGraphics->SwapBuffer();
		lastTime = time;
		++frameCount;
	}

	delete m_sceneManager;
	delete m_gameManager;
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();
	m_iGraphics->Quit();
}

#pragma region Input
void Game::KeyCallback(Key key, KeyAction action)
{
	if(key == Key::M && action == KeyAction::DOWN)
	{
		if(m_iMouse->IsCapturingMouseInput())
		{
			m_iMouse->ReleaseMouseInput();
		}
		else
		{
			m_iMouse->CaptureMouseInput();
		}
	}

	if(action == KeyAction::DOWN)
	{
		m_gameManager->KeyDown(key);
	}
	else if(action == KeyAction::UP)
	{
		m_gameManager->KeyUp(key);
	}

#ifdef WINDOWS_BUILD
	if(key == Key::Z && action == KeyAction::DOWN)
	{
		m_isWireframeMode = !m_isWireframeMode;
		glPolygonMode(GL_FRONT_AND_BACK, m_isWireframeMode ? GL_LINE : GL_FILL);
	}
#endif
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
	glViewport(0, 0, Uknitty::SCRWIDTH, Uknitty::SCRHEIGHT);
}

void Game::ClearScreen()
{
	//glClearColor(189.0f / 256.0f, 224 / 256.0f, 254 / 256.0f, 1.0f);
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Game::Quit()
{
	quitting = true;
}

void Game::ProcessMouse()
{
	for(auto& mouseButton : m_mouseButtons)
	{
		if(m_iMouse->GetButtonDown(mouseButton))
		{
			// If button is pressed but not already tracked
			if(m_mouseButtonStates.find(mouseButton) == m_mouseButtonStates.end())
			{
				m_mouseButtonStates.insert(mouseButton); // Track key
				m_gameManager->MouseButtonDown(mouseButton);
			}
		}
		else
		{
			// If button is not pressed but is tracked
			if(m_mouseButtonStates.find(mouseButton) != m_mouseButtonStates.end())
			{
				m_mouseButtonStates.erase(mouseButton); // Untrack key
				m_gameManager->MouseButtonUp(mouseButton);
			}
		}
	}
}
