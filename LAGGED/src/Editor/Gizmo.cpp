#include "Precomp.h"
#include "Gizmo.h"

#include "ImGui/imgui.h"
#include "ImGuizmo/ImGuizmo.h"

#include "ECS/Scene.h"
#include "ECS/Components/BasicComponents.h"
#include "Core/Engine.h"

namespace LAG
{
	Gizmo::Gizmo() :
		m_GizmoOperation(ImGuizmo::OPERATION::TRANSLATE), m_GizmoMode(ImGuizmo::MODE::WORLD)
	{
	}

	void Gizmo::DrawGizmo(uint32_t targetEntityID, uint32_t cameraEntityID)
	{
		TransformComponent* transform = GetScene()->GetEntity(targetEntityID).GetComponent<TransformComponent>();
		if (transform == nullptr)
			return;
	}

	void Gizmo::DrawViewManipulator(uint32_t cameraEntityID)
	{
		CameraComponent* cameraComp = GetScene()->GetEntity(cameraEntityID).GetComponent<CameraComponent>();

		float viewManipulateRight = ImGui::GetWindowPos().x + (float)ImGui::GetWindowWidth();
		float viewManipulateTop = ImGui::GetWindowPos().y;

		//TODO: To fix crash, first create an actual ImGui window that gizmo shit needs to go onto
		// then set the drawlist
		ImGuizmo::ViewManipulate(&cameraComp->viewMat[0][0], 8.f, ImVec2(0, 0), ImVec2(128, 128), 0x10101010);
	}
}
