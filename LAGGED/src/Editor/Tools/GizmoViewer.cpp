#include "GizmoViewer.h"

#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>
#include <ImGuizmo/ImGuizmo.h>

#include "Core/Engine.h"


#include "ECS/Scene.h"
#include "ECS/Components/BasicComponents.h"
#include "ECS/Components/CameraComponent.h"
#include "Editor/ToolsManager.h"

namespace LAG
{
	GizmoViewer::GizmoViewer(Gizmo& gizmo) :
		ToolBase(ToolCategory::Level, "Gizmo Viewer"), m_Gizmo(gizmo)
	{
	}

	void GizmoViewer::Render()
	{
		GizmoDesc& desc = Gizmo::GetDesc();

		// Options for Operation setting
		int operationVal = static_cast<int>(desc.operation);
		ImGui::SeparatorText("Operation");
		ImGui::RadioButton("Translate", &operationVal, static_cast<int>(GizmoOperation::Translate)); ImGui::SameLine();
		ImGui::RadioButton("Rotate", &operationVal, static_cast<int>(GizmoOperation::Rotate)); ImGui::SameLine();
		ImGui::RadioButton("Scale", &operationVal, static_cast<int>(GizmoOperation::Scale));
		desc.operation = static_cast<GizmoOperation>(operationVal);

		// Options for Mode setting
		int modeIndex = static_cast<int>(desc.mode);
		ImGui::SeparatorText("Mode");
		ImGui::RadioButton("World", &modeIndex, static_cast<int>(GizmoMode::World)); ImGui::SameLine();
		ImGui::RadioButton("Local", &modeIndex, static_cast<int>(GizmoMode::Local));
		desc.mode = static_cast<GizmoMode>(modeIndex);

		ImGui::SeparatorText("Snapping Settings");
		ImGui::Checkbox("Snapping", &desc.useSnap);
		ImGui::Checkbox("Split Snap Axis", &desc.splitSnapAxis);
		if (!desc.splitSnapAxis)
		{
			if (ImGui::DragFloat("Snap Scale", &desc.snapScale.x, 0.1f, 0.f, 32.f, "%.2f"))
				desc.snapScale.z = desc.snapScale.y = desc.snapScale.x;
		}
		else
			ImGui::DragFloat3("Snap Scale", &desc.snapScale[0], 0.1f, 0.f, 32.f, "%.2f");
	}
}
