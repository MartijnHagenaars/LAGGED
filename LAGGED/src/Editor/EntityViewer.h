#pragma once
#include <stdint.h> //TODO: REMOVE! BAD! uint32_t should NOT be used as an entity handle

namespace LAG
{
	class EntityViewer
	{
	public: 
		EntityViewer();
		~EntityViewer();

		void Update();
		void Render(); 

		uint32_t GetSelectedEntityID() const { return m_SelectedEntityID; }

	private:
		uint32_t m_SelectedEntityID = 0;
		float m_BrowserHeight = 0;
	};
}

