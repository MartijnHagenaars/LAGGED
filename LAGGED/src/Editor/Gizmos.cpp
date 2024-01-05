#include "Precomp.h"
#include "Gizmos.h"

//#define IMGUI_DEFINE_MATH_OPERATORS
#include "ImGui/imgui.h"
//#include "ImGui/imgui_internal.h"
#include "ImGuizmo/ImGuizmo.h"

#include "ECS/Scene.h"
#include "Core/Engine.h"

namespace LAG
{
	void Gizmos::DrawViewManipulator(uint32_t cameraEntityID)
	{
		CameraComponent* cameraComp = GetScene()->GetEntity(cameraEntityID).GetComponent<CameraComponent>();

		float viewManipulateRight = ImGui::GetWindowPos().x + (float)ImGui::GetWindowWidth();
		float viewManipulateTop = ImGui::GetWindowPos().y;

		//TODO: To fix crash, first create an actual ImGui window that gizmo shit needs to go onto
		// then set the drawlist
		ImGuizmo::ViewManipulate(&cameraComp->viewMat[0][0], 8.f, ImVec2(0, 0), ImVec2(128, 128), 0x10101010);
	}
}
