#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include "ECS/TypeDefines.h"

namespace LAG
{
	// TODO: Consider moving this somewhere else...
	struct Frame
	{
		float x, y;
		float w, h;
	};

	enum class GizmoOperation : unsigned char
	{
		Translate,
		Rotate,
		Scale
	};

	enum class GizmoMode : unsigned char
	{
		Local,
		World
	};

	struct GizmoDesc
	{
		GizmoOperation operation{ GizmoOperation::Translate };
		GizmoMode mode{ GizmoMode::World };

		bool splitSnapAxis = false;
		bool useSnap = false;

		glm::vec3 snapScale = glm::vec3(1.f);
	};

	class Gizmo
	{
	public:
		static void Render();

		static void SetCameraID(EntityID camID);
		static void SetTargetID(EntityID targetID);

		static GizmoDesc& GetDesc();

	private:
		static inline GizmoDesc s_Desc = {};
		static inline EntityID s_CameraID = ENTITY_NULL; 
		static inline EntityID s_TargetID = ENTITY_NULL;

	};
}