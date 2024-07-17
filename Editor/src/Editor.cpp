#include "Editor.h"
#include "Core/EntryPoint.h"

#include "Core/Engine.h"
#include "Core/Resources/ResourceManager.h"
#include "Core/Resources/Model.h"

#include "ECS/Scene.h"
#include "ECS/Entity.h"
#include "ECS/Components/BasicComponents.h"
#include "ECS/Components/MeshComponent.h"
#include "ECS/Components/CameraComponent.h"

ENTRY_APP(Editor)

void Editor::Initialize()
{
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
}

void Editor::Update()
{
}
