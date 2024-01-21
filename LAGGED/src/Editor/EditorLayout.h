#pragma once

namespace LAG
{
	class Gizmo;
	class EntityViewer;

	class EditorLayout
	{
	public: 
		EditorLayout() = default;
		~EditorLayout() = default;

		void Initialize();
		void Shutdown();

		void Update(float deltaTime);
		void Render();

		Gizmo* m_Gizmo = nullptr;
		EntityViewer* m_EntityViewer = nullptr;
	};
}

