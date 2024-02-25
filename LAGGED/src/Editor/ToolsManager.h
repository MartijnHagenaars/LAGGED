#pragma once
#include <vector>
#include "ToolBase.h"

namespace LAG
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

		void Initialize();
		void Shutdown();

		void Render();

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

