#pragma once
#include "Tools/ToolBase.h"
#include "ECS/Components/CameraComponent.h"

namespace LAG
{
	class Entity;
}

namespace Editor
{
	class Gizmo : public ToolBase
	{
	public: 
		Gizmo();

		void BeginWindow();
		void EndWindow();

		void RenderGizmo(LAG::Entity* targetEntity);
		void RenderViewManipulator();

		void Render() override;

		void SetCameraEntity(LAG::Entity* cameraEntity) { m_CameraEntity = cameraEntity; }
	private:
		int m_GizmoOperation;
		int m_GizmoMode;

		bool m_UseSnap = false;
		bool m_SplitSnapAxis = false;
		glm::vec3 m_SnapScale = glm::vec3(1.f);

		LAG::Entity* m_CameraEntity = nullptr;

		bool m_UseGizmo = true;
	};
}