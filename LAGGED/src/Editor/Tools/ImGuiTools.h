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
		bool WindowBegin() override { return true; }
		void WindowEnd() override {}
	};

	class ImGuiStyleEditor : public ToolBase
	{
	public:
		ImGuiStyleEditor();

		void Render() override;
	};
}
