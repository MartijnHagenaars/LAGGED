#include "Game.h"
#include <iostream>
#include "Core/EntryPoint.h"

#include "Core/Engine.h"
#include "Platform/Base/RendererBase.h"

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
	LAG::Input::AddInputAction(LAG::Input::InputType::LAG_W, LAG::Utility::String("TestButton"));
}

void Game::Shutdown()
{
	printf("Shutfown\n");
}

void Game::Update()
{
	if (LAG::Input::IsActionPressed(LAG::Utility::String("TestButton")))
	{
		LAG::Utility::Logger::Info("Button is being pressed!!");
	}
}
