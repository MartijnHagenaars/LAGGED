#include "Precomp.h"
#include "EditorLayout.h"

#include "EntityViewer.h"

namespace LAG
{
	void EditorLayout::Initialize()
	{
		if(m_EntityViewer == nullptr)
			m_EntityViewer = new EntityViewer();
	}

	void EditorLayout::Shutdown()
	{
		if (m_EntityViewer != nullptr)
			delete m_EntityViewer;
		m_EntityViewer = nullptr;
	}

	void EditorLayout::Update(float deltaTime)
	{
		m_EntityViewer->Update();
	}

	void EditorLayout::Render()
	{
		m_EntityViewer->Render();
	}
}
