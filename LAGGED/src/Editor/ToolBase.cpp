#include "ToolBase.h"
#include <ImGui/imgui.h>

namespace LAG
{
	ToolBase::ToolBase(ToolType type, const std::string& displayName, const std::string& internalName) :
		m_Type(type), m_IsOpen(false),
		m_DisplayName(displayName), m_InternalName(internalName)
	{};

	void ToolBase::WindowBegin()
	{
		ImGui::Begin(GetDisplayName().c_str(), &m_IsOpen, ImGuiWindowFlags_NoCollapse);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	}

	void ToolBase::WindowEnd()
	{
		ImGui::PopStyleVar();
		ImGui::End();
	}

}