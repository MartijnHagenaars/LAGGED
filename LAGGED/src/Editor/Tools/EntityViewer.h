#pragma once
#include "Editor/ToolBase.h"
#include "ECS/Entity.h"

namespace LAG
{
	class Entity;
	class EntityViewer : public ToolBase
	{
	public: 
		EntityViewer();
		~EntityViewer();

		void Render() override; 

		Entity* GetSelectedEntityID() { return &m_SelectedEntity; }

	private:
		void RenderProperties();

		static constexpr int s_MaxNameLength = 128;

		Entity m_SelectedEntity;
		float m_BrowserHeight = 0;

		char m_NewEntityName[s_MaxNameLength];
	};
}

