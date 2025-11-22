#include "ToolsManager.h"

#include <ImGui/imgui.h>

#include "Core/Engine.h"
#include "Platform/Window.h"
#include "ECS/Scene.h"
#include "ECS/Systems/CameraSystem.h"

#include "UI/Gizmo.h"

#include "Utility/Hash.h"

#include "Tools/CameraViewport.h"
#include "Tools/EntityViewer.h"
#include "Tools/GizmoViewer.h"
#include "Tools/ImGuiTools.h"
#include "Tools/LiveFileEditor.h"
#include "Tools/ProfilerViewer.h"

namespace LAG
{
	void ToolsManager::Initialize()
	{
		m_Gizmo = std::make_unique<Gizmo>();

		RegisterTool<EntityViewer>();
		RegisterTool<CameraViewport>();
		RegisterTool<GizmoViewer>(*m_Gizmo);

		RegisterTool<LiveFileEditor>();
		RegisterTool<ImGuiDemoViewer>();
		RegisterTool<ImGuiStyleEditor>();

		RegisterTool<ProfilerViewer>();
	}

	void ToolsManager::Shutdown()
	{
		// m_Tools is an unordered_map
		for (auto& [key, tool] : m_Tools)
		{
			tool.reset();
			tool = nullptr;
		}
	}

	void ToolsManager::PresentEditor()
	{
		BeginDockSpace();
		
		// Create the menu bar, displayed at the top of the window
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
				if (ImGui::MenuItem("Exit"))
					GetWindow()->Close();

                ImGui::EndMenu();
			}

			// Add tool categories to menu bar
			for (int catId = 0; catId < s_ToolCategoryStrings.size(); catId++)
			{
				if (ImGui::BeginMenu(s_ToolCategoryStrings[catId]))
				{
					for (auto const& [key, tool] : m_Tools)
					{
						if (tool->Category() != static_cast<ToolCategory>(catId))
							continue;

						if (ImGui::MenuItem(tool->GetDisplayName().c_str()))
							tool->ToggleTool();
					}
					ImGui::EndMenu();
				}
			}

			ImGui::EndMenuBar();
        }

		// Draw tool windows
		for (auto const& [key, tool] : m_Tools)
		{
			if (tool->IsOpen())
			{
				tool->WindowBegin();
				tool->Render();
				tool->WindowEnd();
			}
		}

		// Draw gizmo
		// TODO: Add toggle for enabling/disabling gizmo
		if (true) 
		{
			EntityID cameraEntityID = CameraSystem::GetActiveCameraEntityID();
			EntityID targetEntityID = static_cast<EntityViewer*>(m_Tools.at(GetTypeHash64<EntityViewer>()).get())->GetSelectedEntityID();
			if (targetEntityID != ENTITY_NULL && cameraEntityID != ENTITY_NULL)
			{
				const ImGuiViewport* viewport = ImGui::GetMainViewport();
				ImGui::SetNextWindowViewport(ImGui::GetMainViewport()->ID);
				ImVec2 viewportSize = viewport->Size;
				ImVec2 viewportPos = viewport->Pos;

				Gizmo::Render(
					cameraEntityID, targetEntityID, 
					Frame{ viewport->Pos.x, viewport->Pos.y, viewport->Size.x, viewport->Size.y }
				);
			}
		}
		

		// TODO: Add Gizmo here...

		////TODO: This has to be fully reworked. This is not good. 
		//Entity cameraEntity = CameraSystem::GetActiveCameraEntity();
		//Entity targetEntity = Entity(); //TODO: Implement properly...
		//if (cameraEntity.Valid() && targetEntity.Valid())
		//{
		//	m_TempGizmoPtr->BeginWindow();
		//	m_TempGizmoPtr->SetCameraEntity(&cameraEntity);
		//	m_TempGizmoPtr->RenderGizmo(&targetEntity);
		//	m_TempGizmoPtr->RenderViewManipulator();
		//	m_TempGizmoPtr->EndWindow();
		//}

		EndDockSpace();

	}

	bool ToolsManager::IsToolOpen(Hash64 toolID)
	{
		if (const auto& it = m_Tools.find(toolID); it != m_Tools.end())
			return it->second->IsOpen();
		else
			CRITICAL("Failed to find tool with ID \"{0}\".", toolID);
		
		return false;
	}

	void ToolsManager::BeginDockSpace()
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(ImGui::GetMainViewport()->ID);

		ImGuiWindowFlags dockspaceFlags =
			ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDecoration |
			ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNav |
			ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoSavedSettings;

		bool isOpen = true;
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		if (!ImGui::Begin("Editor", &isOpen, dockspaceFlags))
			CRITICAL("Failed to load dockspace");

		//Create the dockspace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
			ImGui::DockSpace(ImGui::GetID("Dockspace"), ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
	}

	void ToolsManager::EndDockSpace()
	{
		ImGui::End();
		ImGui::PopStyleVar();
	}
}
