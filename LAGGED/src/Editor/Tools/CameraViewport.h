#pragma once
#include "Editor/ToolBase.h"
#include "glm/vec2.hpp"

namespace LAG
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
		glm::vec2 m_ViewportSize;
		bool isWindowOpen = false;

	};
}