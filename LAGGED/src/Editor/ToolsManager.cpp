#include "ToolsManager.h"

#include "ToolBase.h"
#include "ImGui/imgui.h"

#include "Core/Engine.h"
#include "Platform/Window.h"
#include "ECS/Scene.h"
#include "ECS/Systems/CameraSystem.h"

#include "Tools/Gizmo.h"
#include "Tools/EntityViewer.h"
#include "Tools/ImGuiTools.h"
#include "Tools/CameraViewport.h"
#include "Tools/LiveFileEditor.h"
#include "Tools/ProfilerViewer.h"

namespace LAG
{
	//TODO: Rewrite should remove this
	static Gizmo* m_TempGizmoPtr = nullptr;
	static EntityViewer* m_TempEntViewerPtr = nullptr;

	void ToolsManager::Initialize()
	{
		m_Tools.push_back(m_TempEntViewerPtr = new EntityViewer());
		m_Tools.push_back(m_TempGizmoPtr = new Gizmo());
		m_Tools.push_back(new ImGuiDemoViewer());
		m_Tools.push_back(new ImGuiStyleEditor());
		m_Tools.push_back(new ImGuiStyleEditor());
		m_Tools.push_back(new CameraViewport());
		m_Tools.push_back(new LiveFileEditor());
		m_Tools.push_back(new ProfilerViewer());
	}

	void ToolsManager::Shutdown()
	{
		for (int i = 0; i < m_Tools.size(); i++)
		{
			delete m_Tools[i];
			m_Tools[i] = nullptr;
		}
	}

	void ToolsManager::PresentEditor()
	{
		BeginDockSpace();
		
		//Create the menu bar, displayed at the top of the window
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
				if (ImGui::MenuItem("Exit"))
					GetWindow()->Close();

                ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Level"))
			{
				for (int i = 0; i < m_Tools.size(); i++)
					if (m_Tools[i]->GetType() == ToolType::LEVEL)
					{
						if (ImGui::MenuItem(m_Tools[i]->GetDisplayName().c_str()))
							m_Tools[i]->ToggleTool();
					}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Graphics"))
			{
				for (int i = 0; i < m_Tools.size(); i++)
					if (m_Tools[i]->GetType() == ToolType::GRAPHICS)
					{
						if (ImGui::MenuItem(m_Tools[i]->GetDisplayName().c_str()))
							m_Tools[i]->ToggleTool();
					}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Performance"))
			{
				for (int i = 0; i < m_Tools.size(); i++)
					if (m_Tools[i]->GetType() == ToolType::PERFORMANCE)
					{
						if (ImGui::MenuItem(m_Tools[i]->GetDisplayName().c_str()))
							m_Tools[i]->ToggleTool();
					}
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
        }


		for (int i = 0; i < m_Tools.size(); i++)
		{
			ToolBase* tool = m_Tools[i];
			if (tool->IsOpen())
			{
				tool->WindowBegin();
				tool->Render();
				tool->WindowEnd();
			}
		}

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

	bool ToolsManager::IsToolOpen(const std::string& internalToolName)
	{
		for(const auto& it : m_Tools)
			if (it->GetInternalName() == internalToolName)
				return it->IsOpen();

		//Return false if tool hasn't been found.
		ERROR("Tool with internal name \"{0}\" not found.", internalToolName);
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
