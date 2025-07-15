#pragma once
#include <string>

namespace LAG
{
	enum class ToolType : unsigned char
	{
		LEVEL = 0,
		GRAPHICS,
		PERFORMANCE,
		OTHER
	};

	class ToolBase
	{
	public:
		ToolBase() = delete;
		ToolBase(ToolType type, const std::string& displayName, const std::string& internalName);
		virtual ~ToolBase() = default;

		virtual void Render() = 0;

		virtual void WindowBegin();
		virtual void WindowEnd();

		virtual void OnOpen() {};
		virtual void OnClose() {};

		void OpenTool() { m_IsOpen = true; }
		void ToggleTool() { m_IsOpen = !m_IsOpen; }
		bool IsOpen() const { return m_IsOpen; }

		ToolType GetType() const { return m_Type; }
		const std::string& GetDisplayName() const { return m_DisplayName; }
		const std::string& GetInternalName() const { return m_InternalName; }

	protected:
		bool m_IsOpen = false;

	private:
		std::string m_DisplayName;
		std::string m_InternalName;
		ToolType m_Type;
	};
}