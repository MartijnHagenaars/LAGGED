#pragma once
#include <glm/vec3.hpp>

#include "ECS/TypeDefines.h"
#include "Editor/ToolBase.h"
#include "Editor/UI/Gizmo.h"

namespace LAG
{
	class GizmoViewer : public ToolBase
	{
	public: 
		GizmoViewer() = delete;
		GizmoViewer(Gizmo& gizmo);

		void Render() override;

		void SetCameraEntity(EntityID cameraEntityID) { m_CameraEntityID = cameraEntityID; }
	private:
		Gizmo& m_Gizmo;
		EntityID m_CameraEntityID = ENTITY_NULL;
	};
}