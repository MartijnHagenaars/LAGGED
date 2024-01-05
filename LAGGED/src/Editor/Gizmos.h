#pragma once
#include "ECS/Components/CameraComponent.h"

namespace LAG
{
	class Gizmos
	{
	public: 
		static void DrawViewManipulator(uint32_t cameraEntityID);
	};
}