#pragma once
#include "ECS/Components/CameraComponent.h"

namespace ImGuizmo
{
	enum OPERATION;
	enum MODE;
}

namespace LAG
{
	class Gizmo
	{
	public: 
		Gizmo();
		void RenderGizmo(uint32_t targetEntityID, uint32_t cameraEntityID);
		void RenderViewManipulator(uint32_t cameraEntityID);

		void RenderGizmoProperties();

	private:
		int m_GizmoOperation;
		int m_GizmoMode;

		bool m_UseSnap = false;
		bool m_SplitSnapAxis = false;
		glm::vec3 m_SnapScale = glm::vec3(1.f);

		bool m_UseGizmo = true;
	};
}