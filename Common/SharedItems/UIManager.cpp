#include "UIManager.h"

#include "ImGui-master/backends/imgui_impl_opengl3.h"
#include "ImGui-master/imgui.h"

#include "CountdownTimer.h"
#include "GameplayEvents.h"
#include "GameSharedDependencies.h"
#include "InventoryManager.h"
#include "Player.h"
#include <iostream>
#include <stdexcept>

UIManager::UIManager()
{
	GameSharedDependencies::Set<UIManager>(this);

	m_redScreenEffectTimer = new Uknitty::CountdownTimer(0);
	m_hitMarkerEffectTimer = new Uknitty::CountdownTimer(0);
}

UIManager::~UIManager()
{
	delete m_redScreenEffectTimer;
	delete m_hitMarkerEffectTimer;
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
		case UIManager::MenuType::INVENTORY:
			HUD();
			Inventory();
			break;
		default:
			throw std::runtime_error("Invalid menu type");
	}

	m_hitMarkerEffectTimer->Update(deltaTime);
	UpdateHitMarkerEffect();

	Reticle();

	m_redScreenEffectTimer->Update(deltaTime);
	UpdateRedScreenEffect();

	FPSCounter();

	ImGui::GetFont()->Scale = oldTextSize;
	ImGui::PopFont();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UIManager::UpdateFPS(int fps)
{
	m_fps = fps;
}

void UIManager::PlayRedScreenEffect()
{
	m_redScreenEffectTimer->SetNewDuration(RED_SCREEN_EFFECT_DURATION);
	m_redScreenEffectTimer->Reset();
}

void UIManager::PlayHitMarkerEffect()
{
	m_hitMarkerEffectTimer->SetNewDuration(HIT_MARKER_EFFECT_DURATION);
	m_hitMarkerEffectTimer->Reset();
}

void UIManager::ShowReticle()
{
	m_shouldShowReticle = true;
}

void UIManager::HideReticle()
{
	m_shouldShowReticle = false;
}

void UIManager::MainMenu()
{
	static bool use_work_area = true;
	static ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;

	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(use_work_area ? viewport->WorkPos : viewport->Pos);
	ImGui::SetNextWindowSize(use_work_area ? viewport->WorkSize : viewport->Size);
	ImGui::SetNextWindowBgAlpha(1);

	ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImVec4)ImColor::HSV(0.0f, 0.0f, 0.3f));
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

	ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImVec4)ImColor::HSV(0.0f, 0.0f, 0.3f));
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

			ImGui::PopStyleColor();
		}

		ImGui::End();
	}
}

void UIManager::HUD()
{
	static ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;

	const float bgHeight = 50;
	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(ImVec2(0, viewport->WorkSize.y - bgHeight));
	ImGui::SetNextWindowSize(ImVec2(viewport->WorkSize.x, bgHeight));

	float fontScale = viewport->WorkSize.x / 1600.0f; // 1600 is the width of the window where the font size shouldn't change
	float oldFontScale = ImGui::GetFont()->Scale;
	ImGui::GetFont()->Scale = oldFontScale * fontScale * 0.85f;
	ImGui::PushFont(ImGui::GetFont());

	ImGui::SetNextWindowBgAlpha(0.7f);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImVec4)ImColor(0, 0, 0));
	if(ImGui::Begin("HUD", &m_isMainMenuVisible, flags))
	{
		ImGui::PopStyleColor();

		ImGui::SetCursorPos(ImVec2(10, 12));
		ImGui::Text("HP");

		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_PlotHistogram, (ImVec4)ImColor(0, 200, 0));
		Player* player = GameSharedDependencies::Get<Player>();
		int hpMax = player->GetMaxHp();
		int hp = player->GetHP();
		float progress = static_cast<float>(hp) / static_cast<float>(hpMax);
		ImGui::ProgressBar(progress, ImVec2(200, 0.0f));
		ImGui::PopStyleColor();

		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 20);

		{
			const InventoryManager::Item inventoryItem = GameSharedDependencies::Get<InventoryManager>()->GetCurrentItem();
			std::string itemString = "";
			switch(inventoryItem)
			{
				case InventoryManager::Item::NONE:
					itemString = "NONE";
					break;
				case InventoryManager::Item::GUN:
					itemString = "GUN";
					break;
				case InventoryManager::Item::HAMBURGER:
					itemString = "HAMBURGER";
					break;
				default:
					throw std::runtime_error("Invalid item");
			}
			std::string text = " EQUIPPED:" + itemString;
			ImGui::Text(text.c_str());

			ImGui::SameLine();
			ImGui::SetCursorPosX(500);
			ImGui::Text("     USE->F");
			ImGui::SameLine();
			ImGui::Text("     AIM->RMB");
			ImGui::SameLine();
			ImGui::Text("     SHOOT->LMB");
			ImGui::SameLine();
			ImGui::Text("     INVENTORY->TAB");
			ImGui::SameLine();
			ImGui::Text("    TACTICAL VIEW->V");
			ImGui::SameLine();
			ImGui::Text("    PAUSE->ESC");
		}

		ImGui::GetFont()->Scale = oldFontScale;
		ImGui::PopFont();

		ImGui::End();
	}
}

void UIManager::FPSCounter()
{
	static ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBackground;

	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(0, 0));
	bool trueBool = true;
	if(ImGui::Begin("FPS Counter", &trueBool, flags))
	{
		ImGui::SetCursorPos(ImVec2(0, 0));
		ImGui::Text("FPS: %d", m_fps);

		ImGui::End();
	}
}

void UIManager::Inventory()
{
	static bool use_work_area = true;
	static ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;

	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(use_work_area ? viewport->WorkPos : viewport->Pos);
	ImGui::SetNextWindowSize(use_work_area ? viewport->WorkSize : viewport->Size);
	ImGui::SetNextWindowBgAlpha(0.05f);

	ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImVec4)ImColor(0.0f, 0.0f, 0.0f, 0.05f));
	if(ImGui::Begin("Inventory", &m_isMainMenuVisible, flags))
	{
		ImGui::PopStyleColor();

		const ImVec2 buttonSize = ImVec2(150, 150);
		const float distanceFromCenter = 150;

		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));  // Transparent
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 1, 1, 0.1f));  // Slight hover effect
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1, 1, 1, 0.2f));  // Slight active effect
			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);  // Border size

			{
				std::string text = "Inventory";
				float oldTextSize = ImGui::GetFont()->Scale;
				ImGui::GetFont()->Scale *= 1.5f;
				ImGui::PushFont(ImGui::GetFont());
				auto textWidth = ImGui::CalcTextSize(text.c_str()).x;
				auto textHeight = ImGui::CalcTextSize(text.c_str()).y;
				ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() / 2.0f - textWidth / 2.0f, ImGui::GetWindowHeight() / 2.0f - textHeight / 2.0f));
				ImGui::Text(text.c_str());
				ImGui::GetFont()->Scale = oldTextSize;
				ImGui::PopFont();
			}

			{
				ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() / 2.0f - buttonSize.x / 2.0f, ImGui::GetWindowHeight() / 2.0f - buttonSize.y / 2.0f - distanceFromCenter));
				if(ImGui::Button("##0", buttonSize))
				{
					GameSharedDependencies::Get<InventoryManager>()->SetCurrentItem(InventoryManager::Item::GUN);
					GameSharedDependencies::Get<GameplayEvents>()->OnSelectedInventoryItem();
				}
			}

			{
				ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() / 2.0f - buttonSize.x / 2.0f + distanceFromCenter, ImGui::GetWindowHeight() / 2.0f - buttonSize.y / 2.0f));
				if(ImGui::Button("##1", buttonSize))
				{
					GameSharedDependencies::Get<InventoryManager>()->SetCurrentItem(InventoryManager::Item::HAMBURGER);
					GameSharedDependencies::Get<GameplayEvents>()->OnSelectedInventoryItem();
				}
			}

			{
				ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() / 2.0f - buttonSize.x / 2.0f, ImGui::GetWindowHeight() / 2.0f - buttonSize.y / 2.0f + distanceFromCenter));
				if(ImGui::Button("##2", buttonSize))
				{
					GameSharedDependencies::Get<InventoryManager>()->SetCurrentItem(InventoryManager::Item::NONE);
					GameSharedDependencies::Get<GameplayEvents>()->OnSelectedInventoryItem();
				}
			}

			{
				ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() / 2.0f - buttonSize.x / 2.0f - distanceFromCenter, ImGui::GetWindowHeight() / 2.0f - buttonSize.y / 2.0f));
				if(ImGui::Button("##3", buttonSize))
				{
					GameSharedDependencies::Get<InventoryManager>()->SetCurrentItem(InventoryManager::Item::NONE);
					GameSharedDependencies::Get<GameplayEvents>()->OnSelectedInventoryItem();
				}
			}

			ImGui::PopStyleVar();
			ImGui::PopStyleColor(3);
		}

		ImGui::End();
	}
}

void UIManager::Reticle()
{
	if(!m_shouldShowReticle)
	{
		return;
	}

	static ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs;
	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowBgAlpha(0);
	bool trueBool = true;
	if(ImGui::Begin("Reticle", &trueBool, flags))
	{
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		float size = 5;
		ImU32 color = ImColor(255, 0, 0);
		drawList->AddCircleFilled(center, size, color);
		ImGui::End();
	}
}

void UIManager::UpdateRedScreenEffect()
{
	if(!m_redScreenEffectTimer->IsFinished())
	{
		static ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);

		ImGui::SetNextWindowBgAlpha(1.0f - m_redScreenEffectTimer->GetProgress());
		ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImVec4)ImColor(255, 0, 0));

		bool trueBool = true;
		if(ImGui::Begin("Red Screen Effect", &trueBool, flags))
		{
			ImGui::PopStyleColor();
			ImGui::End();
		}
	}
}

void UIManager::UpdateHitMarkerEffect()
{
	if(!m_hitMarkerEffectTimer->IsFinished())
	{
		static ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs;
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowBgAlpha(0);
		bool trueBool = true;
		if(ImGui::Begin("Hit Marker Effect", &trueBool, flags))
		{
			ImDrawList* drawList = ImGui::GetWindowDrawList();
			ImVec2 center = ImGui::GetMainViewport()->GetCenter();
			float size = 20.0f * (1.0f - m_hitMarkerEffectTimer->GetProgress());
			float thickness = 3;
			ImU32 color = ImColor(255, 0, 0);
			drawList->AddLine(ImVec2(center.x - size, center.y - size), ImVec2(center.x + size, center.y + size), color, thickness);
			drawList->AddLine(ImVec2(center.x - size, center.y + size), ImVec2(center.x + size, center.y - size), color, thickness);
			ImGui::End();
		}
	}
}
