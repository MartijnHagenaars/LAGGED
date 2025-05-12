#pragma once
#include "Editor/ToolBase.h"
#include "ECS/Entity.h"

namespace LAG
{
	class EntityViewer : public ToolBase
	{
	public: 
		EntityViewer();

		void Render() override; 

	private:
		void RenderProperties();

		EntityID m_SelectedEntityID;

		char m_NewEntityName[128];
	};
}

