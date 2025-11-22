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


	void Gizmo::Render(EntityID cameraID, EntityID targetID, const Frame& winFrame)
	{
		Scene* sc = GetScene();
		ImVec2 winPos = ImVec2(winFrame.x, winFrame.y);
		ImVec2 winSize = ImVec2(winFrame.w, winFrame.h);
		ImGuizmo::SetRect(winPos.x, winPos.y, winSize.x, winSize.y);

		// Check if we have a (valid) camera and target transform
		CameraComponent* camera = sc->GetComponent<CameraComponent>(cameraID);
		TransformComponent* targetTransform = sc->GetComponent<TransformComponent>(targetID);
		if (targetTransform == nullptr || camera == nullptr)
			return;

		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(winPos);
		ImGui::SetNextWindowSize(winSize);
		ImGui::SetNextWindowViewport(ImGui::GetMainViewport()->ID);
		ImGui::Begin("Gizmo", 0,
			ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration |
			ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoDocking
		);

		ImGuizmo::Enable(true);
		ImGuizmo::SetDrawlist();

		glm::mat4 targetEntityMatrix = targetTransform->GetTransformMatrix();
		if (ImGuizmo::Manipulate(&camera->viewMat[0][0], &camera->projMat[0][0],
			static_cast<ImGuizmo::OPERATION>(LagToGuizmoOperation[s_Desc.operation]),
			static_cast<ImGuizmo::MODE>(s_Desc.mode),
			&targetEntityMatrix[0][0], NULL,
			s_Desc.useSnap ? &s_Desc.snapScale[0] : NULL))
		{
			targetTransform->SetTransformMatrix(targetEntityMatrix);
		}

		ImGui::End();
	}

	GizmoDesc& Gizmo::GetDesc()
	{
		return s_Desc;
	}
}