#pragma once
#include "Tools/ToolBase.h"

namespace Editor
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
