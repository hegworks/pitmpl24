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
#include "RNG.h"
#include "SceneManager.h"
#include "ShaderProgram.h"
#include "SharedInput.h"
#include "StaticObject.h"
#include "UIManager.h"
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
#include "ShaderType.h"

#endif
#include <stdexcept>

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
	UIManager* uiManager = GameSharedDependencies::Get<UIManager>();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

#pragma region FrameBuffer
	// ShaderProgram
#ifdef WINDOWS_BUILD
	Uknitty::ShaderProgram* frameBufferShaderProgram = new Uknitty::ShaderProgram("../Common/Assets/Shaders/Win/FrameBufferVertex.glsl",
																				  "../Common/Assets/Shaders/Win/FrameBufferFragment.glsl",
																				  Uknitty::ShaderType::FRAME_BUFFER);
#endif // WINDOWS_BUILD
#ifdef Raspberry_BUILD
	Uknitty::ShaderProgram* frameBufferShaderProgram = new Uknitty::ShaderProgram("../Common/Assets/Shaders/Pi/FrameBufferVertex.glsl",
																				  "../Common/Assets/Shaders/Pi/FrameBufferFragment.glsl",
																				  Uknitty::ShaderType::FRAME_BUFFER);
#endif // Raspberry_BUILD

	// Prepare framebuffer rectangle VBO and VAO
	unsigned int rectVAO, rectVBO;
	glGenVertexArrays(1, &rectVAO);
	glGenBuffers(1, &rectVBO);
	glBindVertexArray(rectVAO);
	glBindBuffer(GL_ARRAY_BUFFER, rectVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_screenRectangeles), &m_screenRectangeles, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	frameBufferShaderProgram->Use();
	frameBufferShaderProgram->SetInt("uScreenTexture", 0);
	frameBufferShaderProgram->UnUse();

	// Create Frame Buffer Object
	unsigned int FBO;
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	// Create Framebuffer Texture
#ifdef WINDOWS_BUILD
	int lowResWidth = Uknitty::SCRWIDTH;
	int lowResHeight = Uknitty::SCRHEIGHT;
#endif // WINDOWS_BUILD
#ifdef Raspberry_BUILD
	int lowResWidth = Uknitty::SCRWIDTH * Uknitty::LOW_RES_SCALE;
	int lowResHeight = Uknitty::SCRHEIGHT * Uknitty::LOW_RES_SCALE;
#endif // Raspberry_BUILD
	unsigned int framebufferTexture;
	glGenTextures(1, &framebufferTexture);
	glBindTexture(GL_TEXTURE_2D, framebufferTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, lowResWidth, lowResHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebufferTexture, 0);

	// Create Render Buffer Object
	unsigned int RBO;
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, lowResWidth, lowResHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

	// Error checking framebuffer
	auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if(fboStatus != GL_FRAMEBUFFER_COMPLETE)
		throw std::runtime_error("Framebuffer is not complete");
#pragma endregion

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

		if(gameDeltaTime > 0.2f)
		{
			gameDeltaTime = 0.02f;
		}
#pragma endregion Timing

		glBindFramebuffer(GL_FRAMEBUFFER, FBO); // Bind the custom framebuffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, lowResWidth, lowResHeight);
		glEnable(GL_DEPTH_TEST); // Enable depth testing since it's disabled when drawing the framebuffer rectangle

		ProcessMouse();
		m_gameManager->Update(gameDeltaTime);
		uiManager->UpdateFPS(fps);
		if(m_gameManager->ShouldQuit())
		{
			quitting = true;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0); // Bind the default framebuffer
		glViewport(0, 0, Uknitty::SCRWIDTH, Uknitty::SCRHEIGHT);
		// Draw the framebuffer rectangle
		frameBufferShaderProgram->Use();
		glBindVertexArray(rectVAO);
		glDisable(GL_DEPTH_TEST); // prevents framebuffer rectangle from being discarded
		glBindTexture(GL_TEXTURE_2D, framebufferTexture);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		frameBufferShaderProgram->UnUse();

		uiManager->Update(gameDeltaTime);

		glFlush();
		m_iGraphics->SwapBuffer();
		lastTime = time;
		++frameCount;
	}

	delete m_sceneManager;
	delete m_gameManager;
	frameBufferShaderProgram->Delete();
	delete frameBufferShaderProgram;
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();
	m_iGraphics->Quit();
}

#pragma region Input
void Game::KeyCallback(Key key, KeyAction action)
{
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
