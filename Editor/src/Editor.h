#pragma once
#include "Core/Defines.h"
#include "Core/ApplicationInterface.h"

namespace Editor
{
	class ToolsManager;
	class EditorApp : public LAG::IApplication
	{
	public:
		EditorApp() = default;
		~EditorApp() override = default;

		void Initialize() override;
		void Shutdown() override;

		void Update() override;

		void OnImGui() override;

		ToolsManager* GetToolsManager() const { return m_ToolsManager; }

	private:
		ToolsManager* m_ToolsManager = nullptr;
	};
}

