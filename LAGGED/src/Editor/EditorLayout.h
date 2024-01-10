#pragma once

namespace LAG
{
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

	private:
		EntityViewer* m_EntityViewer = nullptr;
	};
}

