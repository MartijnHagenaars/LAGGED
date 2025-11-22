#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include "ECS/TypeDefines.h"

namespace LAG
{
	// TODO: Consider moving this somewhere else...
	struct Frame
	{
		float x, y; // Position
		float w, h; // Size
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
		GizmoOperation operation{ GizmoOperation::Scale };
		GizmoMode mode{ GizmoMode::World };

		bool splitSnapAxis = false;
		bool useSnap = false;

		glm::vec3 snapScale = glm::vec3(1.f);
	};

	class Gizmo
	{
	public:
		static void Render(EntityID cameraID, EntityID targetID, const Frame& winFrame);

		static GizmoDesc& GetDesc();

	private:
		static inline GizmoDesc s_Desc = {};
	};
}