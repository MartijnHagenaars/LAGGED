#pragma once
#include "Core/Defines.h"
#include "Core/ApplicationInterface.h"

namespace LAGEditor
{
	class ToolsManager;
	class Editor : public LAG::IApplication
	{
	public:
		Editor() = default;
		~Editor() override = default;

		void Initialize() override;
		void Shutdown() override;

		void Update() override;

		void OnImGui() override;

		ToolsManager* GetToolsManager() const { return m_ToolsManager; }

	private:
		ToolsManager* m_ToolsManager = nullptr;
	};
}

