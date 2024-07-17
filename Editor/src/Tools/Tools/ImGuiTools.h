#pragma once
#include "Tools/ToolBase.h"

namespace LAGEditor
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
