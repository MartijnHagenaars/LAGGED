#pragma once
#include "Editor/ToolBase.h"

namespace LAG
{
	class ImGuiDemoViewer : public ToolBase
	{
	public:
		ImGuiDemoViewer();

		void Render() override;
	};

	class ImGuiStyleEditor : public ToolBase
	{
	public:
		ImGuiStyleEditor();

		void Render() override;
	};
}
