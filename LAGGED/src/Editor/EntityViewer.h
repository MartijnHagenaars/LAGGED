#pragma once

namespace LAG
{
	class EntityViewer
	{
	public: 
		EntityViewer();
		~EntityViewer();

		void Update();
		void Render(); 

	private:
		uint32_t m_SelectedEntityID = 0;
		float m_BrowserHeight = 0;
	};
}

