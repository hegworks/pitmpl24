#include "Game.h"

#include "Common.h"
#include "FreeFlyCamera.h"
#include "GeneralCamera.h"
#include "ICamera.h"
#include "IGraphics.h"
#include "IInput.h"
#include "Player.h"
#include "SharedInput.h"
#include "Transform.h"
#include <Model.h>
#include <ShaderProgram.h>

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
#endif

Game::Game(SharedInput* sharedInput, IGraphics* iGraphics) :
	m_sharedInput(sharedInput),
	m_iGraphics(iGraphics)

{
	m_iMouse = m_sharedInput->GetMouse();
	m_iKeyboard = m_sharedInput->GetKeyboard();
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
	io.DisplaySize = ImVec2(SCRWIDTH, SCRHEIGHT);
#pragma endregion imgui

#pragma region Other Initializations
	m_iCamera = new GeneralCamera();
	m_iInputProcessors.push_back(m_iCamera);
	m_iLifeCycles.push_back(m_iCamera);

	m_sharedInput->GetKeyboard()->SetKeyCallback(
		[this](Key key, KeyAction action) { KeyCallback(key, action); }
	);

	Uknitty::ShaderProgram* shaderProgram = new Uknitty::ShaderProgram("../Common/Assets/Shaders/Vertex.glsl", "../Common/Assets/Shaders/Fragment.glsl");

	stbi_set_flip_vertically_on_load(false);
	Uknitty::Model* snake = new Uknitty::Model("../Common/Assets/Models/NakedSnake/NakedSnake.obj");

	m_player = new Player(snake, m_iCamera);
	m_iInputProcessors.push_back(m_player);
	m_iLifeCycles.push_back(m_player);
	m_iRenderables.push_back(m_player);

	GeneralCamera* generalCamera = static_cast<GeneralCamera*>(m_iCamera);
	generalCamera->SetFollowTransform(m_player->m_transform);

	stbi_set_flip_vertically_on_load(false);
	Uknitty::Model* soldier = new Uknitty::Model("../Common/Assets/Models/Soldier/Soldier.obj");

	stbi_set_flip_vertically_on_load(false);
	Uknitty::Model* cube = new Uknitty::Model("../Common/Assets/Models/Primitives/Cube/Cube.obj");

	stbi_set_flip_vertically_on_load(false);
	Uknitty::Model* cube2 = new Uknitty::Model("../Common/Assets/Models/Primitives/Cube/Cube.obj");

	stbi_set_flip_vertically_on_load(false);
	Uknitty::Model* sphere = new Uknitty::Model("../Common/Assets/Models/Primitives/Sphere/Sphere.obj");

	stbi_set_flip_vertically_on_load(false);
	Uknitty::Model* plane = new Uknitty::Model("../Common/Assets/Models/Primitives/Plane/Plane.obj", glm::vec2(5));

	for(auto& iLifeCycle : m_iLifeCycles)
	{
		iLifeCycle->Awake();
	}

	for(auto& iLifeCycle : m_iLifeCycles)
	{
		iLifeCycle->Start();
	}

	glm::mat4 model;
	glm::mat4 identityMat = glm::mat4(1);

#pragma endregion Other Initializations

#pragma region Timing
	auto startTime = std::chrono::system_clock::now();
	auto lastTime = startTime;
	float averageFPS{0};
#pragma endregion Timing

	float degree = 0;

	while(!quitting)
	{
#pragma region Timing
		auto time = std::chrono::system_clock::now();
		std::chrono::duration<float> delta = time - lastTime;
		gameDeltaTime = delta.count();

		std::chrono::duration<float> elapsed = time - startTime;
		if(elapsed.count() > 0.25f && frameCount > 10)
		{
			averageFPS = static_cast<float>(frameCount) / elapsed.count();
			startTime = time;
			frameCount = 0;
		}
#pragma endregion Timing

		for(auto& iInputProcessor : m_iInputProcessors)
		{
			iInputProcessor->MouseCallback(m_iMouse->GetPosition().x, m_iMouse->GetPosition().y);
			iInputProcessor->ProcessInput(m_iKeyboard);
		}

		for(auto& iLifeCycle : m_iLifeCycles)
		{
			iLifeCycle->Update(gameDeltaTime);
		}

		for(auto& iLifeCycle : m_iLifeCycles)
		{
			iLifeCycle->LateUpdate(gameDeltaTime);
		}

		for(auto& iRenderable : m_iRenderables)
		{
			iRenderable->Render();
		}

		// Setup the viewport
		ClearScreen();
		glViewport(0, 0, SCRWIDTH, SCRHEIGHT);

		shaderProgram->Use();
		shaderProgram->SetMat4("uView", m_iCamera->GetView());
		shaderProgram->SetMat4("uProjection", m_iCamera->GetProjection());

		if(generalCamera->GetCameraType() != GeneralCamera::CameraType::FIRST_PERSON)
		{
			shaderProgram->SetMat4("uModel", *m_player->m_transform->GetModelMatrix());
			glDisable(GL_BLEND);
			m_player->m_model->Draw(*shaderProgram);
		}

		model = identityMat;
		model = glm::translate(model, glm::vec3(0, 1, -5));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1, 0, 0));
		model = glm::scale(model, glm::vec3(1));
		shaderProgram->SetMat4("uModel", model);
		glDisable(GL_BLEND);
		soldier->Draw(*shaderProgram);

		model = identityMat;
		model = glm::translate(model, glm::vec3(0, 0, -5));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1, 0, 0));
		model = glm::scale(model, glm::vec3(1));
		shaderProgram->SetMat4("uModel", model);
		glDisable(GL_BLEND);
		cube->Draw(*shaderProgram);

		model = identityMat;
		model = glm::translate(model, glm::vec3(1, 0, -5));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1, 0, 0));
		model = glm::scale(model, glm::vec3(1));
		shaderProgram->SetMat4("uModel", model);
		glDisable(GL_BLEND);
		cube->Draw(*shaderProgram);

		model = identityMat;
		model = glm::translate(model, glm::vec3(3, 0.5, -5));
		shaderProgram->SetMat4("uModel", model);
		glDisable(GL_BLEND);
		sphere->Draw(*shaderProgram);

		model = identityMat;
		model = glm::translate(model, glm::vec3(0, 0, 0));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1, 0, 0));
		model = glm::scale(model, glm::vec3(10));
		shaderProgram->SetMat4("uModel", model);
		glDisable(GL_BLEND);
		plane->Draw(*shaderProgram);

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
		ImGui::Text(std::to_string(averageFPS).c_str());

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

	m_iGraphics->Quit();
}

#pragma region Input
void Game::KeyCallback(Key key, KeyAction action)
{
	if(key == Key::ESCAPE && action == KeyAction::DOWN) Quit();

	if(action == KeyAction::DOWN)
	{
		for(auto& iInputProcessor : m_iInputProcessors)
		{
			iInputProcessor->KeyDown(key);
		}
	}
	else if(action == KeyAction::UP)
	{
		for(auto& iInputProcessor : m_iInputProcessors)
		{
			iInputProcessor->KeyUp(key);
		}
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
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Game::Quit()
{
	quitting = true;
}
