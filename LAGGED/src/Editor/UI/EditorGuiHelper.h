#pragma once
#include <string>

#define EDITOR_UI_WHITESPACE_X 6
#define EDITOR_UI_WHITESPACE_Y 6

namespace LAG
{
	class EditorGui
	{
	public: 
		EditorGui() = delete;

		static void SeparatorText(const std::string& text);
	};
}
