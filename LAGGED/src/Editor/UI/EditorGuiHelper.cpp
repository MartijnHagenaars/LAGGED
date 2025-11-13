#include "EditorGuiHelper.h"

#include <ImGui/imgui.h>

namespace LAG
{
	void EditorGui::SeparatorText(const std::string& text)
	{
		ImGui::Dummy(ImVec2(EDITOR_UI_WHITESPACE_X, EDITOR_UI_WHITESPACE_Y));
		ImGui::SeparatorText(text.c_str());
		ImGui::Dummy(ImVec2(EDITOR_UI_WHITESPACE_X, EDITOR_UI_WHITESPACE_Y));
	}

	EditorGui::Indent::Indent()
	{
		ImGui::Indent(EDITOR_UI_INDENTATION);
	}
	EditorGui::Indent::~Indent()
	{
		ImGui::Unindent(EDITOR_UI_INDENTATION);
	}
}