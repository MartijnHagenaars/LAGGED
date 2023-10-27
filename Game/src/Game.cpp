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

	LAG::ResourceManager::Get().AddResource<LAG::Model>(LAG::Utility::String("res/Assets/Models/Helmet/DamagedHelmet.gltf"));

	LAG::Entity ent1 = LAG::GetEngine().GetScene()->AddEntity();
	ent1.AddComponent<LAG::NameComponent>("First component");
	ent1.AddComponent<LAG::TransformComponent>();

	LAG::Entity ent2 = LAG::GetEngine().GetScene()->AddEntity();
	ent2.AddComponent<LAG::NameComponent>("Second component");

	LAG::Entity ent3 = LAG::GetEngine().GetScene()->AddEntity();
	ent3.AddComponent<LAG::TransformComponent>();

	LAG::GetEngine().GetScene()->Loop<LAG::NameComponent>(
		[](uint32_t entityID, LAG::NameComponent& comp)
		{
			std::cout << "Name: " << comp.name << std::endl;
		});

	LAG::GetEngine().GetScene()->Loop<LAG::NameComponent, LAG::TransformComponent>(
		[](uint32_t entityID, LAG::NameComponent& name, LAG::TransformComponent transform)
		{
			std::cout << "Name: " << name.name << std::endl;
			transform.position = glm::vec3(34.f);
		});

	LAG::GetEngine().GetScene()->RemoveAll();
	LAG::GetEngine().GetScene()->Loop<LAG::NameComponent>(
		[](uint32_t entityID, LAG::NameComponent& comp)
		{
			std::cout << "Name: " << comp.name << std::endl;
		});

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
