#include "ImGuiTools.h"
#include "ImGui/imgui.h"

namespace LAG
{
	ImGuiDemoViewer::ImGuiDemoViewer() : 
		ToolBase(ToolType::GRAPHICS, "ImGui Demo Viewer", "DemoView")
	{
	}

	void ImGuiDemoViewer::Render()
	{
		ImGui::ShowDemoWindow(&m_IsOpen);
	}


	ImGuiStyleEditor::ImGuiStyleEditor() : 
		ToolBase(ToolType::GRAPHICS, "ImGui Style Editor", "StyleEditor")
	{
	}

	void ImGuiStyleEditor::Render()
	{
		ImGui::ShowStyleEditor();
	}
}
