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

	void ToolBase::WindowBegin()
	{
		ImGui::Begin(GetDisplayName().c_str(), &m_IsOpen, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoNav);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	}

	void ToolBase::WindowEnd()
	{
		//ImGui::GetWindowSize();
		ImGui::PopStyleVar();
		ImGui::End();
	}

}