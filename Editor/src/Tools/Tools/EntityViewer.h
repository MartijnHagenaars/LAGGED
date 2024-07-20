#pragma once
#include "Tools/ToolBase.h"
#include "ECS/Entity.h"

namespace LAG
{
	class Entity;
}

namespace LAGEditor
{

	class EntityViewer : public ToolBase
	{
	public: 
		EntityViewer();
		~EntityViewer();

		void Render() override; 

		LAG::Entity* GetSelectedEntityID() { return &m_SelectedEntity; }

	private:
		static constexpr int s_MaxNameLength = 128;

		LAG::Entity m_SelectedEntity;
		float m_BrowserHeight = 0;

		char m_NewEntityName[s_MaxNameLength];
	};
}

