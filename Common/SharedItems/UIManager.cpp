#include "UIManager.h"

#include "ImGui-master/backends/imgui_impl_opengl3.h"
#include "ImGui-master/imgui.h"

#include "GameplayEvents.h"
#include "GameSharedDependencies.h"
#include "Player.h"
#include <iostream>
#include <stdexcept>

UIManager::UIManager()
{
	GameSharedDependencies::Set<UIManager>(this);
}

UIManager::~UIManager()
{
}

void UIManager::Update(float deltaTime)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui::NewFrame();

	float oldTextSize = ImGui::GetFont()->Scale;
	ImGui::GetFont()->Scale *= 1.5f;
	ImGui::PushFont(ImGui::GetFont());

	switch(m_currentMenu)
	{
		case UIManager::MenuType::NONE:
			break;
		case UIManager::MenuType::MAIN_MENU:
			MainMenu();
			break;
		case UIManager::MenuType::PAUSE_MENU:
			PauseMenu();
			break;
		case UIManager::MenuType::WIN_MENU:
			WinMenu();
			break;
		case UIManager::MenuType::LOSE_MENU:
			LoseMenu();
			break;
		case UIManager::MenuType::LOADING_SCREEN:
			LoadingScreen();
			break;
		case UIManager::MenuType::HUD:
			HUD();
			break;
		default:
			throw std::runtime_error("Invalid menu type");
	}

	ImGui::GetFont()->Scale = oldTextSize;
	ImGui::PopFont();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UIManager::MainMenu()
{
	static bool use_work_area = true;
	static ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;

	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(use_work_area ? viewport->WorkPos : viewport->Pos);
	ImGui::SetNextWindowSize(use_work_area ? viewport->WorkSize : viewport->Size);
	ImGui::SetNextWindowBgAlpha(1);

	ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImVec4)ImColor::HSV(0, 0, 0.3));
	if(ImGui::Begin("Main Menu", &m_isMainMenuVisible, flags))
	{
		ImGui::PopStyleColor();

		const ImVec2 buttonSize = ImVec2(150, 50);

		{
			ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor(45, 185, 60));
			ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor(0, 0, 0));

			{
				std::string text = "MAIN MENU";
				float oldTextSize = ImGui::GetFont()->Scale;
				ImGui::GetFont()->Scale *= 2.0f;
				ImGui::PushFont(ImGui::GetFont());
				auto textWidth = ImGui::CalcTextSize(text.c_str()).x;

				ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() / 2 - textWidth / 2, ImGui::GetWindowHeight() / 2 - 100));
				ImGui::Text(text.c_str());

				ImGui::GetFont()->Scale = oldTextSize;
				ImGui::PopFont();
			}

			{
				ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() / 2 - buttonSize.x / 2, ImGui::GetWindowHeight() / 2));
				if(ImGui::Button("Start Game", buttonSize))
				{
					std::cout << "Hello ImGui\n";
					GameSharedDependencies::Get<GameplayEvents>()->OnPressedStartGame();
				}
			}

			{
				ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() / 2 - buttonSize.x / 2, ImGui::GetCursorPosY()));
				if(ImGui::Button("Quit Game", buttonSize))
				{
					GameSharedDependencies::Get<GameplayEvents>()->OnPressedQuit();
				}
			}

			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
		}

		ImGui::End();
	}
}

void UIManager::PauseMenu()
{
	static bool use_work_area = true;
	static ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;

	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(use_work_area ? viewport->WorkPos : viewport->Pos);
	ImGui::SetNextWindowSize(use_work_area ? viewport->WorkSize : viewport->Size);
	ImGui::SetNextWindowBgAlpha(1);

	ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImVec4)ImColor::HSV(0, 0, 0.3));
	if(ImGui::Begin("Pause Menu", &m_isMainMenuVisible, flags))
	{
		ImGui::PopStyleColor();

		const ImVec2 buttonSize = ImVec2(150, 50);

		{
			ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor(45, 185, 60));
			ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor(0, 0, 0));

			{
				std::string text = "GAME PAUSED";
				float oldTextSize = ImGui::GetFont()->Scale;
				ImGui::GetFont()->Scale *= 2.0f;
				ImGui::PushFont(ImGui::GetFont());
				auto textWidth = ImGui::CalcTextSize(text.c_str()).x;

				ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() / 2 - textWidth / 2, ImGui::GetWindowHeight() / 2 - 100));
				ImGui::Text(text.c_str());

				ImGui::GetFont()->Scale = oldTextSize;
				ImGui::PopFont();
			}

			{
				ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() / 2 - buttonSize.x / 2, ImGui::GetWindowHeight() / 2));
				if(ImGui::Button("Resume", buttonSize))
				{
					std::cout << "Resume\n";
					GameSharedDependencies::Get<GameplayEvents>()->OnPressedUnpause();
				}
			}

			{
				ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() / 2 - buttonSize.x / 2, ImGui::GetCursorPosY()));
				if(ImGui::Button("Main Menu", buttonSize))
				{
					GameSharedDependencies::Get<GameplayEvents>()->OnPressedMainMenu();
				}
			}

			{
				ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() / 2 - buttonSize.x / 2, ImGui::GetCursorPosY()));
				if(ImGui::Button("Quit Game", buttonSize))
				{
					GameSharedDependencies::Get<GameplayEvents>()->OnPressedQuit();
				}
			}

			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
		}

		ImGui::End();
	}
}

void UIManager::LoseMenu()
{
	static bool use_work_area = true;
	static ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;

	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(use_work_area ? viewport->WorkPos : viewport->Pos);
	ImGui::SetNextWindowSize(use_work_area ? viewport->WorkSize : viewport->Size);
	ImGui::SetNextWindowBgAlpha(1);

	ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImVec4)ImColor(190, 50, 0));
	if(ImGui::Begin("Lose Menu", &m_isMainMenuVisible, flags))
	{
		ImGui::PopStyleColor();

		const ImVec2 buttonSize = ImVec2(150, 50);

		{
			ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor(45, 185, 60));
			ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor(255, 255, 255));

			{
				std::string text = "YOU DIED";
				float oldTextSize = ImGui::GetFont()->Scale;
				ImGui::GetFont()->Scale *= 2.0f;
				ImGui::PushFont(ImGui::GetFont());
				auto textWidth = ImGui::CalcTextSize(text.c_str()).x;

				ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() / 2 - textWidth / 2, ImGui::GetWindowHeight() / 2 - 100));
				ImGui::Text(text.c_str());

				ImGui::GetFont()->Scale = oldTextSize;
				ImGui::PopFont();
			}

			{
				ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() / 2 - buttonSize.x / 2, ImGui::GetWindowHeight() / 2));
				if(ImGui::Button("Main Menu", buttonSize))
				{
					GameSharedDependencies::Get<GameplayEvents>()->OnPressedMainMenu();
				}
			}

			{
				ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() / 2 - buttonSize.x / 2, ImGui::GetCursorPosY()));
				if(ImGui::Button("Quit Game", buttonSize))
				{
					GameSharedDependencies::Get<GameplayEvents>()->OnPressedQuit();
				}
			}

			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
		}

		ImGui::End();
	}
}

void UIManager::WinMenu()
{
	static bool use_work_area = true;
	static ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;

	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(use_work_area ? viewport->WorkPos : viewport->Pos);
	ImGui::SetNextWindowSize(use_work_area ? viewport->WorkSize : viewport->Size);
	ImGui::SetNextWindowBgAlpha(1);

	ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImVec4)ImColor(130, 190, 0));
	if(ImGui::Begin("Win Menu", &m_isMainMenuVisible, flags))
	{
		ImGui::PopStyleColor();

		const ImVec2 buttonSize = ImVec2(150, 50);

		{
			ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor(30, 30, 30));
			ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor(255, 255, 255));

			{
				std::string text = "YOU WON!";
				float oldTextSize = ImGui::GetFont()->Scale;
				ImGui::GetFont()->Scale *= 2.0f;
				ImGui::PushFont(ImGui::GetFont());
				auto textWidth = ImGui::CalcTextSize(text.c_str()).x;

				ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() / 2 - textWidth / 2, ImGui::GetWindowHeight() / 2 - 100));
				ImGui::Text(text.c_str());

				ImGui::GetFont()->Scale = oldTextSize;
				ImGui::PopFont();
			}

			{
				ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() / 2 - buttonSize.x / 2, ImGui::GetWindowHeight() / 2));
				if(ImGui::Button("Main Menu", buttonSize))
				{
					GameSharedDependencies::Get<GameplayEvents>()->OnPressedMainMenu();
				}
			}

			{
				ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() / 2 - buttonSize.x / 2, ImGui::GetCursorPosY()));
				if(ImGui::Button("Quit Game", buttonSize))
				{
					GameSharedDependencies::Get<GameplayEvents>()->OnPressedQuit();
				}
			}

			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
		}

		ImGui::End();
	}
}

void UIManager::LoadingScreen()
{
	static bool use_work_area = true;
	static ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;

	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(use_work_area ? viewport->WorkPos : viewport->Pos);
	ImGui::SetNextWindowSize(use_work_area ? viewport->WorkSize : viewport->Size);
	ImGui::SetNextWindowBgAlpha(1);

	ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImVec4)ImColor(10, 60, 10));
	if(ImGui::Begin("Loading Screen", &m_isMainMenuVisible, flags))
	{
		ImGui::PopStyleColor();

		const ImVec2 buttonSize = ImVec2(150, 50);

		{
			{
				ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor(255, 255, 255));
				std::string text = "LOADING...";
				float oldTextSize = ImGui::GetFont()->Scale;
				ImGui::GetFont()->Scale *= 2.0f;
				ImGui::PushFont(ImGui::GetFont());
				auto textWidth = ImGui::CalcTextSize(text.c_str()).x;

				ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() / 2 - textWidth / 2, ImGui::GetWindowHeight() / 2));
				ImGui::Text(text.c_str());

				ImGui::GetFont()->Scale = oldTextSize;
				ImGui::PopFont();
			}

			ImGui::PopStyleColor();
		}

		ImGui::End();
	}
}

void UIManager::HUD()
{
	static bool use_work_area = true;
	static ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;

	const float bgHeight = 50;
	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(ImVec2(0, viewport->WorkSize.y - bgHeight));
	ImGui::SetNextWindowSize(ImVec2(viewport->WorkSize.x, bgHeight));

	ImGui::SetNextWindowBgAlpha(0.7);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImVec4)ImColor(0, 0, 0));
	if(ImGui::Begin("HUD", &m_isMainMenuVisible, flags))
	{
		ImGui::PopStyleColor();

		const ImVec2 hpBgSize = ImVec2(150, 50);
		ImVec2 hpInnerSize = ImVec2(100, 50);

		ImGui::SetCursorPos(ImVec2(10, 12));
		ImGui::Text("HP");

		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_PlotHistogram, (ImVec4)ImColor(0, 200, 0));
		Player* player = GameSharedDependencies::Get<Player>();
		float hpMax = player->GetMaxHp();
		float hp = player->GetHP();
		float progress = hp / hpMax;
		ImGui::ProgressBar(progress, ImVec2(200, 0.0f));
		ImGui::PopStyleColor();

		ImGui::End();
	}
}
