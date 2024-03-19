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
		ToolBase(ToolType type, const std::string& name) : m_Type(type), m_IsOpen(false), m_Name(name) {};
		virtual ~ToolBase() {};

		virtual void Render() = 0;

		void OpenTool() { m_IsOpen = true; }
		void ToggleTool() { m_IsOpen = !m_IsOpen; }
		bool IsOpen() const { return m_IsOpen; }

		ToolType GetType() const { return m_Type; }
		const std::string& GetName() const { return m_Name; }

	protected:
		bool m_IsOpen = false;

	private:
		std::string m_Name;
		ToolType m_Type;
	};
}