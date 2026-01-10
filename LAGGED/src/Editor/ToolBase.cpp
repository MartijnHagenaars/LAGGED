#include "ToolBase.h"
#include <ImGui/imgui.h>

namespace LAG
{
	ToolBase::ToolBase(ToolCategory cat, const std::string&& displayName) :
		m_DisplayName(displayName), 
		m_Category(cat), 
		m_IsOpen(false)
	{
	};

	bool ToolBase::WindowBegin()
	{
		bool res = ImGui::Begin(DisplayName().c_str(), &m_IsOpen, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoNav);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		return res;
	}

	void ToolBase::WindowEnd()
	{
		ImGui::PopStyleVar();
		ImGui::End();
	}
}