#include "ToolsManager.h"

#include "ToolBase.h"
#include "ImGui/imgui.h"

#include "Core/Engine.h"
#include "ECS/Scene.h"
#include "Platform/Base/Window/WindowManager.h"

#include "Tools/Gizmo.h"
#include "Tools/EntityViewer.h"
#include "Tools/ImGuiTools.h"
#include "Tools/CameraViewport.h"

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
	}

	void ToolsManager::Shutdown()
	{
		for (int i = 0; i < m_Tools.size(); i++)
		{
			delete m_Tools[i];
			m_Tools[i] = nullptr;
		}
	}

	void ToolsManager::Render()
	{
		BeginDockSpace();
		
		//Create the menu bar, displayed at the top of the window
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
				if (ImGui::MenuItem("Exit"))
					GetEngine().GetWindowManager()->RemoveWindow(GetEngine().GetWindowManager()->GetPrimaryWindow());

                ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Level"))
			{
				for (int i = 0; i < m_Tools.size(); i++)
					if (m_Tools[i]->GetType() == ToolType::LEVEL)
					{
						if (ImGui::MenuItem(m_Tools[i]->GetName().c_str()))
							m_Tools[i]->ToggleTool();
					}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Graphics"))
			{
				for (int i = 0; i < m_Tools.size(); i++)
					if (m_Tools[i]->GetType() == ToolType::GRAPHICS)
					{
						if (ImGui::MenuItem(m_Tools[i]->GetName().c_str()))
							m_Tools[i]->ToggleTool();
					}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Performance"))
			{
				for (int i = 0; i < m_Tools.size(); i++)
					if (m_Tools[i]->GetType() == ToolType::PERFORMANCE)
					{
						if (ImGui::MenuItem(m_Tools[i]->GetName().c_str()))
							m_Tools[i]->ToggleTool();
					}
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
        }

		EndDockSpace();


		Entity cameraEntity;
		GetScene()->Loop<CameraComponent>([&cameraEntity](Entity entity, CameraComponent& camera) 
			{
				if (camera.isActive) 
					cameraEntity = entity; 
			});

		for (int i = 0; i < m_Tools.size(); i++)
			if (m_Tools[i]->IsOpen())
				m_Tools[i]->Render();

		//TODO: This has to be fully reworked. This is not good. 
		if (cameraEntity.IsValid() && m_TempEntViewerPtr->GetSelectedEntityID()->IsValid())
		{
			m_TempGizmoPtr->RenderGizmo(m_TempEntViewerPtr->GetSelectedEntityID(), &cameraEntity);
		}

	}

	void ToolsManager::BeginDockSpace()
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);

		ImGuiWindowFlags dockspaceFlags =
			ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDecoration |
			ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNav |
			ImGuiWindowFlags_NoBackground;

		bool isOpen = true;
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		if (!ImGui::Begin("Editor", &isOpen, dockspaceFlags))
			Logger::Critical("Failed to load dockspace");

		//Create the dockspace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
			ImGui::DockSpace(ImGui::GetID("Dockspace"), ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_NoDockingOverCentralNode | ImGuiDockNodeFlags_PassthruCentralNode);

	}

	void ToolsManager::EndDockSpace()
	{
		ImGui::End();
		ImGui::PopStyleVar();
	}
}