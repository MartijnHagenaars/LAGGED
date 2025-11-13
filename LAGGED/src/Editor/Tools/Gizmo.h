#pragma once
#include <glm/vec3.hpp>
#include "Editor/ToolBase.h"
#include "ECS/TypeDefines.h"

namespace LAG
{
	class Gizmo : public ToolBase
	{
	public: 
		Gizmo();

		void BeginWindow();
		void EndWindow();

		void RenderGizmo(EntityID targetEntityID);
		void RenderViewManipulator();

		void Render() override;

		void SetCameraEntity(EntityID cameraEntityID) { m_CameraEntityID = cameraEntityID; }
	private:
		int m_GizmoOperation;
		int m_GizmoMode;

		bool m_UseSnap = false;
		bool m_SplitSnapAxis = false;
		glm::vec3 m_SnapScale = glm::vec3(1.f);

		EntityID m_CameraEntityID = ENTITY_NULL;
	};
}