#pragma once
#include "Editor/ToolBase.h"
#include "ECS/Components/CameraComponent.h"

namespace LAG
{
	class Entity;
	class Gizmo : public ToolBase
	{
	public: 
		Gizmo();

		void BeginWindow();
		void EndWindow();

		void RenderGizmo(Entity* targetEntity);
		void RenderViewManipulator(uint32_t cameraEntityID);

		void Render() override;

		void SetCameraEntity(Entity* cameraEntity) { m_CameraEntity = cameraEntity; }
	private:
		int m_GizmoOperation;
		int m_GizmoMode;

		bool m_UseSnap = false;
		bool m_SplitSnapAxis = false;
		glm::vec3 m_SnapScale = glm::vec3(1.f);

		Entity* m_CameraEntity = nullptr;

		bool m_UseGizmo = true;
	};
}