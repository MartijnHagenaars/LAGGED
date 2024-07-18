#include "Editor.h"
#include "Core/EntryPoint.h"

//Engine headers for demonstrating some functionality
#include "Core/Engine.h"
#include "Core/Resources/ResourceManager.h"
#include "Core/Resources/Model.h"

#include "ECS/Scene.h"
#include "ECS/Entity.h"
#include "ECS/Components/BasicComponents.h"
#include "ECS/Components/MeshComponent.h"
#include "ECS/Components/CameraComponent.h"

#include "Tools/ToolsManager.h"


ENTRY_APP(LAGEditor::Editor)

namespace LAGEditor
{
	void Editor::Initialize()
	{
		INFO("Initializing editor...");

		m_ToolsManager = new ToolsManager();
		m_ToolsManager->Initialize();

		LAG::GetResourceManager()->AddResource<LAG::Model>(LAG::HashedString("res/Assets/Models/Helmet/DamagedHelmet.gltf"));

		//Load an entity with a model and a transform
		LAG::Entity ent1 = LAG::GetEngine().GetScene()->AddEntity("Helmet");
		ent1.AddComponent<LAG::TransformComponent>()->SetPosition(glm::vec3(0.f, 0.f, -5.f));
		ent1.AddComponent<LAG::MeshComponent>("res/Assets/Models/Helmet/DamagedHelmet.gltf");
		ent1.AddComponent<LAG::SinWaveComponent>(6.f, 1.f);

		LAG::Entity ent5 = LAG::GetEngine().GetScene()->AddEntity("Camera");
		ent5.AddComponent<LAG::TransformComponent>()->SetPosition(glm::vec3(0.f, -10.f, 0.f));
		ent5.GetComponent<LAG::TransformComponent>()->SetRotation(glm::vec3(1.57f, 0.f, 0.f));
		ent5.AddComponent<LAG::CameraComponent>()->movementSpeed = 80.f;
	}

	void Editor::Shutdown()
	{
		INFO("Shutting down editor...");

		if (m_ToolsManager != nullptr)
		{
			m_ToolsManager->Shutdown();
			delete m_ToolsManager;
		}
		m_ToolsManager = nullptr;
	}

	void Editor::Update()
	{

	}

	void Editor::OnImGui()
	{
		m_ToolsManager->Render();
	}
}
