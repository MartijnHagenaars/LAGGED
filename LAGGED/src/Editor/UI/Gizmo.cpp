#include "Gizmo.h"

#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>
#include <ImGuizmo/ImGuizmo.h>

#include "Core/Engine.h"
#include "ECS/Scene.h"
#include "ECS/Components/BasicComponents.h"
#include "ECS/Components/CameraComponent.h"

namespace LAG
{
	std::unordered_map<unsigned short, LAG::GizmoOperation> GuizmoToLagOperation =
	{
		{ ImGuizmo::TRANSLATE, LAG::GizmoOperation::Translate },
		{ ImGuizmo::ROTATE, LAG::GizmoOperation::Rotate },
		{ ImGuizmo::SCALE, LAG::GizmoOperation::Scale }
	};

	std::unordered_map<LAG::GizmoOperation, unsigned short> LagToGuizmoOperation =
	{
		{ LAG::GizmoOperation::Translate, ImGuizmo::TRANSLATE},
		{ LAG::GizmoOperation::Rotate, ImGuizmo::ROTATE },
		{ LAG::GizmoOperation::Scale, ImGuizmo::SCALE }
	};


	void Gizmo::Render()
	{
		if (!s_TargetID || !s_CameraID)
			return;

		// Check if we have a (valid) camera and target transform
		Scene* sc = GetScene();
		CameraComponent* camera = sc->GetComponent<CameraComponent>(s_CameraID);
		TransformComponent* targetTransform = sc->GetComponent<TransformComponent>(s_TargetID);
		if (targetTransform == nullptr || camera == nullptr)
			return;

		ImVec2 viewportPos = ImGui::GetWindowPos();
		ImVec2 viewportSize = ImGui::GetWindowSize();
		ImVec2 clipMax = ImVec2(viewportPos.x + viewportSize.x, viewportPos.y + viewportSize.y);

		ImGuizmo::Enable(true);
		ImGuizmo::SetRect(viewportPos.x, viewportPos.y, viewportSize.x, viewportSize.y);

		const auto& sceneDrawList = ImGui::GetWindowDrawList();

		ImGuizmo::SetDrawlist(sceneDrawList);
		sceneDrawList->PushClipRect(viewportPos, { viewportPos.x + viewportSize.x, viewportPos.y + viewportSize.y });

		glm::mat4 targetEntityMatrix = targetTransform->GetTransformMatrix();
		if (ImGuizmo::Manipulate(&camera->viewMat[0][0], &camera->projMat[0][0],
			static_cast<ImGuizmo::OPERATION>(LagToGuizmoOperation[s_Desc.operation]),
			static_cast<ImGuizmo::MODE>(s_Desc.mode),
			&targetEntityMatrix[0][0], NULL,
			s_Desc.useSnap ? &s_Desc.snapScale[0] : NULL))
		{
			targetTransform->SetTransformMatrix(targetEntityMatrix);
		}
	}

	void Gizmo::SetCameraID(EntityID camID)
	{
		s_CameraID = camID;
	}

	void Gizmo::SetTargetID(EntityID targetID)
	{
		s_TargetID = targetID;
	}

	GizmoDesc& Gizmo::GetDesc()
	{
		return s_Desc;
	}
}