#pragma once
#include "Editor/ToolBase.h"

namespace LAG
{
	class ImGuiDemoViewer : public ToolBase
	{
	public:
		ImGuiDemoViewer();

		void Render() override;

		// Overriding functions with empty implementations since 
		// ImGui::ShowDemoWindow implements ImGui::Begin and ImGui::End itself...
		void WindowBegin() override {}
		void WindowEnd() override {}
	};

	class ImGuiStyleEditor : public ToolBase
	{
	public:
		ImGuiStyleEditor();

		void Render() override;
	};
}
