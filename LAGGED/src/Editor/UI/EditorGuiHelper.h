#pragma once
#include <string>

#define EDITOR_UI_WHITESPACE_X 6
#define EDITOR_UI_WHITESPACE_Y 6

#define EDITOR_UI_INDENTATION 16

namespace LAG
{
	class EditorGui
	{
	public: 
		EditorGui() = delete;

		static void SeparatorText(const std::string& text);

		class Indent
		{
		public:
			Indent();
			~Indent();
		};
	};
}
