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
	LAG::Input::AddInputAction(LAG::Input::InputType::LAG_LMB, LAG::Utility::String("LMB"));

	LAG::GetResourceManager()->AddResource<LAG::Model>(LAG::Utility::String("res/Assets/Models/Helmet/DamagedHelmet.gltf"));
	LAG::GetResourceManager()->AddResource<LAG::Model>(LAG::Utility::String("res/Assets/Models/Avocado/Avocado.gltf"));
	LAG::GetResourceManager()->AddResource<LAG::Model>(LAG::Utility::String("res/Assets/Models/BoomBox/BoomBox.gltf"));

	//Load an entity with a model and a transform
	LAG::Entity ent1 = LAG::GetEngine().GetScene()->AddEntity();
	ent1.AddComponent<LAG::NameComponent>("Helmet");
	ent1.AddComponent<LAG::TransformComponent>()->position = glm::vec3(0.f, 0.f, -5.f);
	//ent1.GetComponent<LAG::TransformComponent>()->scale = glm::vec3(75.f);
	//ent1.AddComponent<LAG::MeshComponent>("res/Assets/Models/BoomBox/BoomBox.gltf");
	ent1.AddComponent<LAG::MeshComponent>("res/Assets/Models/Helmet/DamagedHelmet.gltf");

	LAG::Entity ent2 = LAG::GetEngine().GetScene()->AddEntity();
	ent2.AddComponent<LAG::NameComponent>("Light1");
	ent2.AddComponent<LAG::TransformComponent>()->position = glm::vec3(0.f, 0.f, -5.f);
	auto ent2Light = ent2.AddComponent<LAG::LightComponent>();
	ent2Light->lightAttenuation = 5.f;
	ent2Light->lightIntensity = 2.f;

	LAG::Entity ent3 = LAG::GetEngine().GetScene()->AddEntity();
	ent3.AddComponent<LAG::NameComponent>("Light1");
	ent3.AddComponent<LAG::TransformComponent>()->position = glm::vec3(-5.f, -2.f, -7.f);
	auto ent3Light = ent3.AddComponent<LAG::LightComponent>();
	ent3Light->lightAttenuation = 0.1f;
	ent3Light->lightIntensity = 2.f;
	ent3Light->lightColor = glm::vec3(0.f, 1.f, 0.f);

	LAG::Entity ent4 = LAG::GetEngine().GetScene()->AddEntity();
	ent4.AddComponent<LAG::NameComponent>("Light1");
	ent4.AddComponent<LAG::TransformComponent>()->position = glm::vec3(5.f, 2.f, -7.f);
	auto ent4Light = ent4.AddComponent<LAG::LightComponent>();
	ent4Light->lightAttenuation = 0.1f;
	ent4Light->lightIntensity = 2.f;
	ent4Light->lightColor = glm::vec3(1.f, 0.f, 0.f);

	LAG::Entity ent5 = LAG::GetEngine().GetScene()->AddEntity();
	ent5.AddComponent<LAG::NameComponent>("Camera");
	ent5.AddComponent<LAG::TransformComponent>()->position = glm::vec3(0.f);
	ent5.AddComponent<LAG::CameraComponent>();
}

void Game::Shutdown()
{
	printf("Shutdown\n");
}

void Game::Update()
{
	if (LAG::Input::IsActionPressedOnce(LAG::Utility::String("LMB")))
	{
		float x, y;
		LAG::Input::GetMousePosition(x, y);
		LAG::Utility::Logger::Info("Clickity at {0}, {1}!\n", x, y);
	}
}
