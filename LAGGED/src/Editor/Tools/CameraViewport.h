#pragma once
#include "Editor/ToolBase.h"

namespace LAG
{
	class CameraViewport : public ToolBase
	{
	public:
		CameraViewport();
		~CameraViewport();

		void Render() override;

	};
}