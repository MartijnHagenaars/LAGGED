#include "CameraViewport.h"
#include "Platform/Base/Renderer/RendererBase.h"

#include "ECS/Scene.h"
#include "ECS/Systems/CameraSystem.h"
#include "ECS/Components/CameraComponent.h"

#include "ImGui/imgui.h"

namespace LAG
{
	CameraViewport::CameraViewport() : ToolBase(ToolType::LEVEL, "Camera viewport")
	{
	}

	CameraViewport::~CameraViewport() {}

	void CameraViewport::Render()
	{
		ImGui::Begin("Viewport", &m_IsOpen);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

		CameraComponent* cameraComp = CameraSystem::GetActiveCameraComponent();
		glm::vec2 winSize = glm::vec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y);
		if (m_ViewportSize != winSize)
		{
			m_ViewportSize = winSize;
			cameraComp->hasCameraDimensionChanged = true;
			cameraComp->m_Framebuffer->Resize(m_ViewportSize);
			cameraComp->m_Framebuffer->UseWindowSize(false); //TODO: Set window resize back to true when disabled
			
			Renderer::OnResize(cameraComp->m_Framebuffer->GetSize().x, cameraComp->m_Framebuffer->GetSize().y);
		}

		ImGui::PopStyleVar();
		ImGui::Image(cameraComp->m_Framebuffer->GetEditorHandle(), ImGui::GetContentRegionAvail(), ImVec2(0.f, 1.f), ImVec2(1.f, 0.f));
		ImGui::End();
	}
}
