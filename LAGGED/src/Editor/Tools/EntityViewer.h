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
		Entity m_SelectedEntity;
		float m_BrowserHeight = 0;
	};
}

