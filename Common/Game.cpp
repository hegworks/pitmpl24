#include "Game.h"

#include "Camera.h"
#include "Common.h"
#include "FreeFlyCamera.h"
#include "IGraphics.h"
#include "IInput.h"
#include "SharedInput.h"
#include <Model.h>
#include <ShaderProgram.h>

#include "ImGui-master/backends/imgui_impl_opengl3.h"
#include "ImGui-master/imgui.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <chrono>

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
	stbi_set_flip_vertically_on_load(true);

	m_camera = new FreeFlyCamera();
	m_sharedInput->GetKeyboard()->SetKeyCallback(
		[this](Key key, KeyAction action) { KeyCallback(key, action); }
	);

	ShaderProgram* shaderProgram = new ShaderProgram("../Common/Assets/Shaders/Vertex.glsl", "../Common/Assets/Shaders/Fragment.glsl");
	Model* backpack = new Model("../Common/Assets/Models/Backpack/backpack.obj");

	glm::mat4 model;
	glm::mat4 identityMat = glm::mat4(1);

#pragma endregion Other Initializations

#pragma region Timing
	auto startTime = std::chrono::system_clock::now();
	auto lastTime = startTime;
	float averageFPS{0};
#pragma endregion Timing

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

		ProcessInput();

		m_camera->Update(gameDeltaTime);

		// Setup the viewport
		ClearScreen();
		glViewport(0, 0, SCRWIDTH, SCRHEIGHT);

		shaderProgram->Use();
		shaderProgram->SetMat4("uView", m_camera->GetView());
		shaderProgram->SetMat4("uProjection", m_camera->GetProjection());

		model = identityMat;
		model = glm::translate(model, glm::vec3(-2, 4, -2));
		model = glm::scale(model, glm::vec3(1));
		shaderProgram->SetMat4("uModel", model);
		backpack->Draw(*shaderProgram);

#pragma region imgui
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();
		ImGui::SetNextWindowBgAlpha(0.2f);
		ImGui::SetNextWindowPos(ImVec2(10, 100));
		ImGuiWindowFlags window_flags = /*ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar  | */ ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoScrollbar;

		static bool open = true;
		// open a new window
		ImGui::Begin("helpinfo ", &open, window_flags);
		ImGui::SetWindowFontScale(1.6f);

		// this can be anything
		ImGui::Text("Hi there");
		ImGui::Text("Hi to you too");

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
SharedInput* Game::GetInput() const
{
	return m_sharedInput;
}

void Game::ProcessInput()
{
	SharedInput* input = GetInput();
	IMouse* mouse = input->GetMouse();
	IKeyboard* keyboard = input->GetKeyboard();

	//if(keyboard->GetKey(Key::W))
	//{
	//	printf("we pressed W\n");
	//}

	//if(mouse->GetButtonDown(MouseButtons::LEFT))
	//{
	//	printf("we pressed mouse left\n");
	//}

	m_camera->MouseCallback(mouse->GetPosition().x, mouse->GetPosition().y);
	m_camera->ProcessInput(keyboard);
}

void Game::KeyCallback(Key key, KeyAction action)
{
	// Implement your key callback logic here
	if(action == KeyAction::DOWN)
	{
		switch(key)
		{
			case Key::ESCAPE:
				Quit();
				break;
			default:
				break;
		}
	}
	else
	{
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

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);

	glCullFace(GL_BACK);
	glViewport(0, 0, SCRWIDTH, SCRHEIGHT);
}

void Game::ClearScreen()
{
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

btDiscreteDynamicsWorld* Game::World() const
{
	return world;
}

void Game::Quit()
{
	quitting = true;
}
