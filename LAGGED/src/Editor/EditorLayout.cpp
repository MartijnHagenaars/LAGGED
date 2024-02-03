#include "EditorLayout.h"

#include "Gizmo.h"
#include "EntityViewer.h"

#include "Core/Engine.h"
#include "ECS/Scene.h"

namespace LAG
{
	void EditorLayout::Initialize()
	{
		if(m_EntityViewer == nullptr)
			m_EntityViewer = new EntityViewer();

		if (m_Gizmo == nullptr)
			m_Gizmo = new Gizmo();
	}

	void EditorLayout::Shutdown()
	{
		if (m_EntityViewer != nullptr)
			delete m_EntityViewer;
		m_EntityViewer = nullptr;

		if (m_Gizmo != nullptr)
			delete m_Gizmo;
		m_Gizmo = nullptr;
	}

	void EditorLayout::Update(float deltaTime)
	{
		m_EntityViewer->Update();
	}

	void EditorLayout::Render()
	{
		m_EntityViewer->Render();

		Entity cameraEntity;
		GetScene()->Loop<CameraComponent>([&cameraEntity](Entity entity, CameraComponent& camera) 
			{
				if (camera.isActive) 
					cameraEntity = entity; 
			});

		if (cameraEntity.IsValid() && m_EntityViewer->GetSelectedEntityID()->IsValid())
		{
			m_Gizmo->RenderGizmo(m_EntityViewer->GetSelectedEntityID(), &cameraEntity);
			m_Gizmo->RenderGizmoProperties();
		}

	}
}
