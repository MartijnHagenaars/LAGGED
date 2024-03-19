#include "CameraViewport.h"

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

		ImVec2 vMin = ImGui::GetWindowContentRegionMin();
		ImVec2 vMax = ImGui::GetWindowContentRegionMax();

		vMin.x += ImGui::GetWindowPos().x;
		vMin.y += ImGui::GetWindowPos().y;
		vMax.x += ImGui::GetWindowPos().x;
		vMax.y += ImGui::GetWindowPos().y;
		ImVec2 size = ImVec2(vMax.x - vMin.x, vMax.y - vMin.y);

		CameraComponent* cameraComp = CameraSystem::GetActiveCameraComponent();
		ImGui::Image(cameraComp->m_Framebuffer->GetEditorHandle(), size, ImVec2(0.f, 1.f), ImVec2(1.f, 0.f));

		ImGui::End();
	}
}
