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
		void DrawGizmo(uint32_t targetEntityID, uint32_t cameraEntityID);

		void DrawViewManipulator(uint32_t cameraEntityID);

	private:
		ImGuizmo::OPERATION m_GizmoOperation;
		ImGuizmo::MODE m_GizmoMode;
	};
}