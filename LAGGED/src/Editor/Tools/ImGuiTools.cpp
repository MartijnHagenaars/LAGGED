#include "ImGuiTools.h"
#include "ImGui/imgui.h"

namespace LAG
{
	ImGuiDemoViewer::ImGuiDemoViewer() : 
		ToolBase(ToolCategory::Graphics, "ImGui Demo Viewer")
	{
	}

	void ImGuiDemoViewer::Render()
	{
		ImGui::ShowDemoWindow(&m_IsOpen);
	}

	ImGuiStyleEditor::ImGuiStyleEditor() : 
		ToolBase(ToolCategory::Graphics, "ImGui Style Editor")
	{
	}

	void ImGuiStyleEditor::Render()
	{
		ImGui::ShowStyleEditor();
	}
}
