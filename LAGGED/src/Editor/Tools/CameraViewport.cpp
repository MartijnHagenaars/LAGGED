#include "CameraViewport.h"
#include "Platform/Base/Renderer/RendererBase.h"

#include "ECS/Scene.h"
#include "ECS/Systems/CameraSystem.h"
#include "ECS/Components/CameraComponent.h"

#include "ImGui/imgui.h"

namespace LAG
{
	CameraViewport::CameraViewport() : ToolBase(ToolType::LEVEL, "Camera Viewport", "Viewport")
	{
	}

	CameraViewport::~CameraViewport()
	{
	}

	void CameraViewport::Render()
	{
		ImGui::Begin(GetInternalName().c_str(), &m_IsOpen, ImGuiWindowFlags_NoCollapse);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

		CameraComponent* cameraComp = CameraSystem::GetActiveCameraComponent();
		glm::uvec2 winSize = glm::uvec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y);
		if (m_ViewportSize != winSize)
		{
			//Ensure that a valid width and height are used
			if (winSize.x <= 0) winSize.x = 1;
			if (winSize.y <= 0) winSize.y = 1;

			m_ViewportSize = winSize;
			cameraComp->hasCameraDimensionChanged = true;
			cameraComp->m_Framebuffer->Resize(m_ViewportSize);
			cameraComp->m_Framebuffer->UseWindowSize(false); //TODO: Set window resize back to true when disabled

			Renderer::OnResize(cameraComp->m_Framebuffer->GetSize().x, cameraComp->m_Framebuffer->GetSize().y);
		}

		ImGui::PopStyleVar();
		ImGui::Image(cameraComp->m_Framebuffer->GetEditorHandle(), ImGui::GetContentRegionAvail(), ImVec2(0.f, 1.f), ImVec2(1.f, 0.f));
		ImGui::End();

		if (m_IsOpen != isWindowOpen)
		{
			isWindowOpen = m_IsOpen;
			cameraComp->m_Framebuffer->UseWindowSize(!m_IsOpen);
			cameraComp->m_Framebuffer->Resize(m_ViewportSize);
			Renderer::OnResize(cameraComp->m_Framebuffer->GetSize().x, cameraComp->m_Framebuffer->GetSize().y);
		}
	}

	void CameraViewport::OnOpen()
	{
	}

	void CameraViewport::OnClose()
	{
	}
}
