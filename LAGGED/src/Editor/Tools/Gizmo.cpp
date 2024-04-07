#include "Gizmo.h"

#include "Core/Engine.h"
#include "Editor/ToolsManager.h"

#include "ImGui/imgui.h"
#include "ImGuizmo/ImGuizmo.h"

#include "ECS/Scene.h"
#include "ECS/Entity.h"
#include "ECS/Components/BasicComponents.h"
#include "Core/Engine.h"

#include "ImGui/imgui_internal.h"

namespace LAG
{
	Gizmo::Gizmo() :
		ToolBase(ToolType::LEVEL, "Gizmo Properties", "GizmoProperties"), m_GizmoOperation(ImGuizmo::OPERATION::TRANSLATE), m_GizmoMode(ImGuizmo::MODE::WORLD)
	{
	}

	void Gizmo::BeginWindow()
	{
		//Get the position of the viewport
		ImVec2 viewportPos;
		ImVec2 viewportSize;

		if (GetToolsManager()->IsToolOpen("CamView"))
		{
			//Not a fan of this, but cannot come up with another approach.
			ImGuiContext* currentContext = ImGui::GetCurrentContext();
			for (const auto& it : currentContext->WindowsFocusOrder)
			{
				if (strcmp(it->Name, "CamView") == 0)
				{
					ImGui::SetNextWindowViewport(it->ID);
					ImGuizmo::SetRect(it->Pos.x, it->Pos.y, it->Size.x, it->Size.y);
					viewportSize = it->Size;
					viewportPos = it->Pos;
					break;
				}
			}
		}
		else
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowViewport(ImGui::GetMainViewport()->ID);
			ImGuizmo::SetRect(viewport->Pos.x, viewport->Pos.y, viewport->Size.x, viewport->Size.y);
			viewportSize = viewport->Size;
			viewportPos = viewport->Pos;
		}


		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewportPos);
		ImGui::SetNextWindowSize(viewportSize);
		ImGui::SetNextWindowViewport(ImGui::GetMainViewport()->ID);
		ImGui::Begin("GizmoView", 0, ImGuiWindowFlags_NoBackground |
			ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs |
			ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDocking
		);
	}

	void Gizmo::EndWindow()
	{
		ImGui::End();
	}

	void Gizmo::RenderGizmo(Entity* targetEntity)
	{
		TransformComponent* targetTransform = targetEntity->GetComponent<TransformComponent>();
		CameraComponent* camera = m_CameraEntity->GetComponent<CameraComponent>();
		if (targetTransform == nullptr || camera == nullptr)
		{
			ImGuizmo::Enable(false);
			return;
		}

		ImGuizmo::Enable(true);
		ImGuizmo::SetDrawlist();

		glm::mat4 targetEntityMatrix = targetTransform->GetTransformMatrix();
		if (ImGuizmo::Manipulate(&camera->viewMat[0][0], &camera->projMat[0][0],
			static_cast<ImGuizmo::OPERATION>(m_GizmoOperation),
			static_cast<ImGuizmo::MODE>(m_GizmoMode),
			&targetEntityMatrix[0][0], NULL,
			m_UseSnap ? &m_SnapScale[0] : NULL))
		{
			targetTransform->SetTransformMatrix(targetEntityMatrix);
		}
	}

	void Gizmo::RenderViewManipulator(uint32_t cameraEntityID)
	{
		TransformComponent* transform = GetScene()->GetEntity(cameraEntityID).GetComponent<TransformComponent>();
		CameraComponent* cameraComp = GetScene()->GetEntity(cameraEntityID).GetComponent<CameraComponent>();

		float viewManipulateRight = ImGui::GetWindowPos().x + (float)ImGui::GetWindowWidth();
		float viewManipulateTop = ImGui::GetWindowPos().y;

		glm::mat4 viewMat = transform->GetTransformMatrix();
		ImGuizmo::ViewManipulate(&viewMat[0][0], 8.f, ImVec2(0, 0), ImVec2(128, 128), 0x10101010);
	}

	void Gizmo::Render()
	{
		ImGui::Begin("Gizmo Properties", &m_IsOpen);

		ImGui::Checkbox("Enable Gizmo", &m_UseGizmo);

		ImGui::SeparatorText("Operation");
		ImGui::RadioButton("Translate", &m_GizmoOperation, static_cast<int>(ImGuizmo::OPERATION::TRANSLATE)); ImGui::SameLine();
		ImGui::RadioButton("Rotate", &m_GizmoOperation, static_cast<int>(ImGuizmo::OPERATION::ROTATE)); ImGui::SameLine();
		ImGui::RadioButton("Scale", &m_GizmoOperation, static_cast<int>(ImGuizmo::OPERATION::SCALE));

		ImGui::SeparatorText("Mode");
		ImGui::RadioButton("World", &m_GizmoMode, static_cast<int>(ImGuizmo::MODE::WORLD)); ImGui::SameLine();
		ImGui::RadioButton("Local", &m_GizmoMode, static_cast<int>(ImGuizmo::MODE::LOCAL));

		ImGui::SeparatorText("Snapping Settings");
		ImGui::Checkbox("Snapping", &m_UseSnap);
		ImGui::Checkbox("Split Snap Axis", &m_SplitSnapAxis);
		if (!m_SplitSnapAxis)
		{
			if (ImGui::DragFloat("Snap Scale", &m_SnapScale.x, 0.1f, 0.f, 32.f, "%.2f"))
				m_SnapScale.z = m_SnapScale.y = m_SnapScale.x;
		}
		else
			ImGui::DragFloat3("Snap Scale", &m_SnapScale[0], 0.1f, 0.f, 32.f, "%.2f");

		ImGui::End();
	}
}
