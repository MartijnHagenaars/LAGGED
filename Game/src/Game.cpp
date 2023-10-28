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
	LAG::Input::AddInputAction(LAG::Input::InputType::LAG_W, LAG::Utility::String("UpButton"));
	LAG::Input::AddInputAction(LAG::Input::InputType::LAG_S, LAG::Utility::String("DownButton"));
	LAG::Input::AddInputAction(LAG::Input::InputType::LAG_LMB, LAG::Utility::String("LMB"));

	LAG::GetResourceManager()->AddResource<LAG::Model>(LAG::Utility::String("res/Assets/Models/Helmet/DamagedHelmet.gltf"));

	//Load an entity with a model and a transform
	LAG::Entity ent1 = LAG::GetEngine().GetScene()->AddEntity();
	ent1.AddComponent<LAG::NameComponent>("Helmet");
	ent1.AddComponent<LAG::TransformComponent>()->position = glm::vec3(0.f, 0.f, -5.f);
	ent1.AddComponent<LAG::MeshComponent>("res/Assets/Models/Helmet/DamagedHelmet.gltf");
}

void Game::Shutdown()
{
	printf("Shutdown\n");
}

void Game::Update()
{
	if (LAG::Input::IsActionPressedOnce(LAG::Utility::String("UpButton")))
	{
		LAG::Utility::Logger::Info("W is being pressed, but only detected once!");
	}
	else if (LAG::Input::IsActionPressed(LAG::Utility::String("DownButton")))
	{
		LAG::Utility::Logger::Info("S is being pressed!");
	}
	else if (LAG::Input::IsActionPressedOnce(LAG::Utility::String("LMB")))
	{
		float x, y;
		LAG::Input::GetMousePosition(x, y);
		LAG::Utility::Logger::Info("Clickity at {0}, {1}!\n", x, y);
	}
}
