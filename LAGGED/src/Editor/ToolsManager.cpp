#include "ToolsManager.h"

#include "Tools/Gizmo.h"
#include "Tools/EntityViewer.h"
#include "Tools/ImGuiTools.h"

#include "Core/Engine.h"
#include "ECS/Scene.h"
#include "Platform/Base/Window/WindowManager.h"

//TODO: Potentially move
#include "ImGui/imgui.h"

namespace LAG
{
	void ToolsManager::Initialize()
	{
		if(m_EntityViewer == nullptr)
			m_EntityViewer = new EntityViewer();

		if (m_Gizmo == nullptr)
			m_Gizmo = new Gizmo();

		if (m_ImGuiDemoViewer == nullptr)
			m_ImGuiDemoViewer = new ImGuiDemoViewer;

		if (m_ImGuiStyleEditor == nullptr)
			m_ImGuiStyleEditor = new ImGuiStyleEditor;
	}

	void ToolsManager::Shutdown()
	{
		if (m_EntityViewer != nullptr)
			delete m_EntityViewer;
		m_EntityViewer = nullptr;

		if (m_Gizmo != nullptr)
			delete m_Gizmo;
		m_Gizmo = nullptr;
	}

	//TODO: Remove?
	void ToolsManager::Update(float deltaTime)
	{
	}

	void ToolsManager::Render()
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

		//Create the menu bar, displayed at the top of the window
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
				if (ImGui::MenuItem("Exit"))
					GetEngine().GetWindowManager()->RemoveWindow(GetEngine().GetWindowManager()->GetPrimaryWindow());

                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        ImGui::End();
		ImGui::PopStyleVar();


		m_EntityViewer->Render();
		m_Gizmo->Render();

		m_ImGuiDemoViewer->Render();
		m_ImGuiStyleEditor->Render();

		Entity cameraEntity;
		GetScene()->Loop<CameraComponent>([&cameraEntity](Entity entity, CameraComponent& camera) 
			{
				if (camera.isActive) 
					cameraEntity = entity; 
			});

		if (cameraEntity.IsValid() && m_EntityViewer->GetSelectedEntityID()->IsValid())
		{
			m_Gizmo->RenderGizmo(m_EntityViewer->GetSelectedEntityID(), &cameraEntity);
		}

	}
}
