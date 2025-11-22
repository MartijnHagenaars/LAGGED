#pragma once
#include "Editor/ToolBase.h"
#include "ECS/TypeDefines.h"

namespace LAG
{
	class EntityViewer : public ToolBase
	{
	public: 
		EntityViewer();

		void Render() override; 
		
		EntityID GetSelectedEntityID() const { return m_SelectedEntityID; }

	private:
		void RenderProperties();

		EntityID m_SelectedEntityID;

		char m_NewEntityName[128];
	};
}

