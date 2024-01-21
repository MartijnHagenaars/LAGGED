#include "Game.h"
#include <iostream>
#include "Core/EntryPoint.h"

#include "Core/Engine.h"
#include "Platform/Base/Renderer/RendererBase.h"

#include "Core/Memory/ResourcePtr.h"
#include "Core/Resources/ResourceManager.h"
#include "Core/Resources/Model.h"

#include "ECS/Scene.h"
#include "ECS/Components/BasicComponents.h"
#include "ECS/Components/MeshComponent.h"
#include "ECS/Components/CameraComponent.h"
#include "ECS/Components/LightComponent.h"

ENTRY_APP(Game)

static uint32_t meshEntityID = 0;

Game::Game()
{
	std::cout << "Construct..." << std::endl;
}

Game::~Game()
{
	printf("Destruct\n");
}

void Game::Initialize()
{
	LAG::Input::AddInputAction(LAG::Input::InputType::LAG_W, LAG::HashedString("UpButton"));
	LAG::Input::AddInputAction(LAG::Input::InputType::LAG_S, LAG::HashedString("DownButton"));
	LAG::Input::AddInputAction(LAG::Input::InputType::LAG_LMB, LAG::HashedString("LMB"));

	LAG::GetResourceManager()->AddResource<LAG::Model>(LAG::HashedString("res/Assets/Models/Helmet/DamagedHelmet.gltf"));
	//LAG::GetResourceManager()->AddResource<LAG::Model>(LAG::HashedString("res/Assets/Models/Avocado/Avocado.gltf"));
	//LAG::GetResourceManager()->AddResource<LAG::Model>(LAG::HashedString("res/Assets/Models/BoomBox/BoomBox.gltf"));

	//Load an entity with a model and a transform
	LAG::Entity ent1 = LAG::GetEngine().GetScene()->AddEntity("Helmet");
	ent1.AddComponent<LAG::TransformComponent>()->SetPosition(glm::vec3(0.f, 0.f, -5.f));
	//ent1.GetComponent<LAG::TransformComponent>()->scale = glm::vec3(75.f);
	//ent1.AddComponent<LAG::MeshComponent>("res/Assets/Models/BoomBox/BoomBox.gltf");
	ent1.AddComponent<LAG::MeshComponent>("res/Assets/Models/Helmet/DamagedHelmet.gltf");
	ent1.AddComponent<LAG::SinWaveComponent>(6.f, 1.f);
	meshEntityID = ent1.GetEntityID();

	LAG::Entity ent2 = LAG::GetEngine().GetScene()->AddEntity("Light1");
	ent2.AddComponent<LAG::TransformComponent>()->SetPosition(glm::vec3(0.f, 0.f, -5.f));
	auto ent2Light = ent2.AddComponent<LAG::LightComponent>();
	ent2Light->lightAttenuation = 5.f;
	ent2Light->lightIntensity = 2.f;

	LAG::Entity ent3 = LAG::GetEngine().GetScene()->AddEntity("Light2");
	ent3.AddComponent<LAG::TransformComponent>()->SetPosition(glm::vec3(-5.f, -2.f, -7.f));
	auto ent3Light = ent3.AddComponent<LAG::LightComponent>();
	ent3Light->lightAttenuation = 0.1f;
	ent3Light->lightIntensity = 2.f;
	ent3Light->lightColor = glm::vec3(0.f, 1.f, 0.f);

	LAG::Entity ent4 = LAG::GetEngine().GetScene()->AddEntity("Light3");
	ent4.AddComponent<LAG::TransformComponent>()->SetPosition(glm::vec3(5.f, 2.f, -7.f));
	auto ent4Light = ent4.AddComponent<LAG::LightComponent>();
	ent4Light->lightAttenuation = 0.1f;
	ent4Light->lightIntensity = 2.f;
	ent4Light->lightColor = glm::vec3(1.f, 0.f, 0.f);

	LAG::Entity ent5 = LAG::GetEngine().GetScene()->AddEntity();
	ent5.AddComponent<LAG::NameComponent>("Camera");
	ent5.AddComponent<LAG::TransformComponent>()->position = glm::vec3(0.f, -120.f, 0.f);
	ent5.AddComponent<LAG::TransformComponent>()->rotation = glm::vec3(1.57f, 0.f, 0.f);
	ent5.AddComponent<LAG::CameraComponent>();
}

void Game::Shutdown()
{
	printf("Shutdown\n");
}

void Game::Update()
{
	if (LAG::Input::IsActionPressedOnce(LAG::HashedString("LMB")))
	{
		float x, y;
		LAG::Input::GetMousePosition(x, y);
		LAG::Logger::Info("Clickity at {0}, {1}!\n", x, y);
	}

	LAG::Entity meshEntity = LAG::GetScene()->GetEntity(meshEntityID);
	
	//meshEntity.GetComponent<LAG::TransformComponent>()->SetPosition(
	//	glm::vec3(meshEntity.GetComponent<LAG::SinWaveComponent>()->sinValue, 
	//	meshEntity.GetComponent<LAG::TransformComponent>()->GetPosition().y, 
	//	meshEntity.GetComponent<LAG::TransformComponent>()->GetPosition().x
	//));


}
