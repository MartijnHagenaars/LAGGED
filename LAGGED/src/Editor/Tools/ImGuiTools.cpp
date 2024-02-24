#include "ImGuiTools.h"
#include "ImGui/imgui.h"

namespace LAG
{
	ImGuiDemoViewer::ImGuiDemoViewer() : 
		ToolBase(ToolType::GRAPHICS, "ImGui Demo Viewer")
	{
	}

	void ImGuiDemoViewer::Render()
	{
		ImGui::ShowDemoWindow(&m_IsOpen);
	}


	ImGuiStyleEditor::ImGuiStyleEditor() : 
		ToolBase(ToolType::GRAPHICS, "ImGui Style Editor")
	{

	}

	void ImGuiStyleEditor::Render()
	{
		ImGui::Begin("ImGui Style Editor", &m_IsOpen);
		ImGui::ShowStyleEditor();
		ImGui::End();
	}
}
