#pragma once
#include "Tools/ToolBase.h"
#include "glm/vec2.hpp"

namespace Editor
{
	class CameraViewport : public ToolBase
	{
	public:
		CameraViewport();
		~CameraViewport();

		void Render() override;

		void OnOpen() override;
		void OnClose() override;

	private:
		glm::uvec2 m_ViewportSize;
		bool isWindowOpen = false;

	};
}