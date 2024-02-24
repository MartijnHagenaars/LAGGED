#pragma once

namespace LAG
{
	class Gizmo;
	class EntityViewer;

	class ImGuiDemoViewer;
	class ImGuiStyleEditor;

	class ToolsManager
	{
	public: 
		ToolsManager() = default;
		~ToolsManager() = default;

		void Initialize();
		void Shutdown();

		void Update(float deltaTime);
		void Render();

		Gizmo* m_Gizmo = nullptr;
		EntityViewer* m_EntityViewer = nullptr;

		ImGuiDemoViewer* m_ImGuiDemoViewer = nullptr;
		ImGuiStyleEditor* m_ImGuiStyleEditor = nullptr;
	};
}

