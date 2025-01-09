#include "UIManager.h"

#include "ImGui-master/backends/imgui_impl_opengl3.h"
#include "ImGui-master/imgui.h"

#include "GameplayEvents.h"
#include "GameSharedDependencies.h"
#include <iostream>

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

	if(m_isMainMenuVisible)
	{
		static bool use_work_area = true;
		static ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;

		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(use_work_area ? viewport->WorkPos : viewport->Pos);
		ImGui::SetNextWindowSize(use_work_area ? viewport->WorkSize : viewport->Size);

		const ImVec2 buttonSize = ImVec2(100, 50);

		if(ImGui::Begin("Main Menu", &m_isMainMenuVisible, flags))
		{
			ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() / 2 - buttonSize.x / 2, ImGui::GetWindowHeight() / 2 - buttonSize.y));
			if(ImGui::Button("Start Game", ImVec2(100, 50)))
			{
				std::cout << "Hello ImGui\n";
			}
			ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() / 2 - buttonSize.x / 2, ImGui::GetCursorPosY()));
			if(ImGui::Button("Quit Game", ImVec2(100, 50)))
			{
				GameSharedDependencies::Get < GameplayEvents>()->OnPressedQuit();
			}
			ImGui::End();
		}
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
