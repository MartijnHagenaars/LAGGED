#pragma once
#include <string>
#include <array>

#include "Utility/Hash.h"

namespace LAG
{
	// Define the ToolCategory enum, containing the various tool categories
	DEFINE_ENUM_AND_STRINGS(ToolCategory, Level, Graphics, Debug);

	class ToolBase
	{
	public:
		ToolBase() = delete;
		ToolBase(ToolCategory category, const std::string&& displayName);
		virtual ~ToolBase() = default;

		virtual void Render() = 0;

		virtual void WindowBegin();
		virtual void WindowEnd();

		void OpenTool() { m_IsOpen = true; }
		void ToggleTool() { m_IsOpen = !m_IsOpen; }
		bool IsOpen() const { return m_IsOpen; }

		/// <summary>
		/// Returns the tool type / category
		/// </summary>
		ToolCategory Category() const { return m_Category; }

		const std::string& GetDisplayName() const { return m_DisplayName; }



	private:
		std::string m_DisplayName;
		ToolCategory m_Category;

	protected:
		bool m_IsOpen = false;
	};
}