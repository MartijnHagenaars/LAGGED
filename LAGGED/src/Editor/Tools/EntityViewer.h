#pragma once
#include "Editor/ToolBase.h"
#include "ECS/Entity.h"

namespace LAG
{
	class EntityViewer : public ToolBase
	{
	public: 
		EntityViewer();
		~EntityViewer();

		void Render() override; 

		//Entity GetSelectedEntity() const { return m_SelectedEntity; }

	private:
		void RenderProperties();

		Entity* m_SelectedEntity;
		float m_BrowserHeight = 0;

		char m_NewEntityName[128];
	};
}

