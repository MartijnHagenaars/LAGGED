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
	LAG::Input::AddInputAction(LAG::Input::InputType::LAG_W, LAG::Utility::String("UpButton"));
	LAG::Input::AddInputAction(LAG::Input::InputType::LAG_S, LAG::Utility::String("DownButton"));
}

void Game::Shutdown()
{
	printf("Shutfown\n");
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
}
