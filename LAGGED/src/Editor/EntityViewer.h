#pragma once
#include "ECS/Entity.h"

namespace LAG
{
	class Entity;
	class EntityViewer
	{
	public: 
		EntityViewer();
		~EntityViewer();

		void Update();
		void Render(); 

		Entity* GetSelectedEntityID() { return &m_SelectedEntity; }

	private:
		Entity m_SelectedEntity;
		float m_BrowserHeight = 0;
	};
}

