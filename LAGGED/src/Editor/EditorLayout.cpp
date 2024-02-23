#include "EditorLayout.h"

#include "Gizmo.h"
#include "EntityViewer.h"

#include "Core/Engine.h"
#include "ECS/Scene.h"

//TODO: Potentially move
#include "ImGui/imgui.h"

namespace LAG
{
	void EditorLayout::Initialize()
	{
		if(m_EntityViewer == nullptr)
			m_EntityViewer = new EntityViewer();

		if (m_Gizmo == nullptr)
			m_Gizmo = new Gizmo();
	}

	void EditorLayout::Shutdown()
	{
		if (m_EntityViewer != nullptr)
			delete m_EntityViewer;
		m_EntityViewer = nullptr;

		if (m_Gizmo != nullptr)
			delete m_Gizmo;
		m_Gizmo = nullptr;
	}

	void EditorLayout::Update(float deltaTime)
	{
		m_EntityViewer->Update();
	}

	void EditorLayout::Render()
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);

		ImGuiWindowFlags dockspaceFlags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDecoration;
		dockspaceFlags |= ImGuiWindowFlags_NoMove;
		dockspaceFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNav;

		bool isOpen = true;
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		if (!ImGui::Begin("Editor", &isOpen, dockspaceFlags))
			Logger::Critical("Failed to load dockspace");

		//Create the dockspace
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
			ImGui::DockSpace(ImGui::GetID("Dockspace"), ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_NoDockingOverCentralNode);

		//Create the menu bar, displayed at the top of the window
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Exit")) {}

                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        ImGui::End();
		ImGui::PopStyleVar();


		m_EntityViewer->Render();

		Entity cameraEntity;
		GetScene()->Loop<CameraComponent>([&cameraEntity](Entity entity, CameraComponent& camera) 
			{
				if (camera.isActive) 
					cameraEntity = entity; 
			});

		if (cameraEntity.IsValid() && m_EntityViewer->GetSelectedEntityID()->IsValid())
		{
			m_Gizmo->RenderGizmo(m_EntityViewer->GetSelectedEntityID(), &cameraEntity);
			m_Gizmo->RenderGizmoProperties();
		}

	}
}
