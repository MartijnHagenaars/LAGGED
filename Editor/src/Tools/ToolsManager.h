#pragma once
#include <vector>
#include "ToolBase.h"

namespace LAGEditor
{
	class Gizmo;
	class EntityViewer;

	class ImGuiDemoViewer;
	class ImGuiStyleEditor;
	class ToolBase;

	class ToolsManager
	{
	public: 
		ToolsManager() = default;
		~ToolsManager() = default;

		/// <summary>
		/// Sets up all tools. Needs to be called on startup when the user wants to use the editor. 
		/// </summary>
		void Initialize();

		/// <summary>
		/// Removes all tools. Needs to be called on shutdown of the editor.
		/// </summary>
		void Shutdown();

		void Render();

		/// <summary>
		/// Check if a tool is open. Uses the internal tool name for lookup. 
		/// </summary>
		/// <param name="internalToolName">Internal tool name that is used to look up the tool.</param>
		/// <returns>Boolean that determines whether the tool is open or not.</returns>
		bool IsToolOpen(const std::string& internalToolName);

	private:
		void BeginDockSpace();
		void EndDockSpace();

		std::vector<ToolBase*> m_Tools;

		//Gizmo* m_Gizmo = nullptr;
		//EntityViewer* m_EntityViewer = nullptr;

		//ImGuiDemoViewer* m_ImGuiDemoViewer = nullptr;
		//ImGuiStyleEditor* m_ImGuiStyleEditor = nullptr;
	};
}

