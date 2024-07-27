#include "CameraViewport.h"

#include "Core/Engine.h"
#include "Platform/Renderer.h"

#include "ECS/Scene.h"
#include "ECS/Systems/CameraSystem.h"
#include "ECS/Components/CameraComponent.h"

#include "ImGui/imgui.h"

namespace Editor
{
	CameraViewport::CameraViewport() : ToolBase(ToolType::LEVEL, "Camera Viewport", "CamView")
	{
	}

	CameraViewport::~CameraViewport()
	{
	}

	void CameraViewport::Render()
	{
		ImGui::Begin(GetInternalName().c_str(), &m_IsOpen, ImGuiWindowFlags_NoCollapse);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

		LAG::CameraComponent* cameraComp = LAG::CameraSystem::GetActiveCameraComponent();
		glm::uvec2 winSize = glm::uvec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y);
		if (m_ViewportSize != winSize)
		{
			//Ensure that a valid width and height are used
			if (winSize.x <= 0) winSize.x = 1;
			if (winSize.y <= 0) winSize.y = 1;

			m_ViewportSize = winSize;
			cameraComp->hasCameraDimensionChanged = true;
			cameraComp->framebuffer->Resize(m_ViewportSize);
			cameraComp->framebuffer->UseWindowSize(false); //TODO: Set window resize back to true when disabled

			LAG::GetRenderer()->OnResize(cameraComp->framebuffer->GetSize().x, cameraComp->framebuffer->GetSize().y);
		}

		ImGui::PopStyleVar();
		ImGui::Image(cameraComp->framebuffer->GetEditorHandle(), ImGui::GetContentRegionAvail(), ImVec2(0.f, 1.f), ImVec2(1.f, 0.f));
		ImGui::End();

		if (m_IsOpen != isWindowOpen)
		{
			isWindowOpen = m_IsOpen;
			cameraComp->framebuffer->UseWindowSize(!m_IsOpen);
			cameraComp->framebuffer->Resize(m_ViewportSize);
			LAG::GetRenderer()->OnResize(cameraComp->framebuffer->GetSize().x, cameraComp->framebuffer->GetSize().y);
		}
	}

	void CameraViewport::OnOpen()
	{
	}

	void CameraViewport::OnClose()
	{
	}
}
