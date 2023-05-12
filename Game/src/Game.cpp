#include "Game.h"
#include <iostream>
#include "Core/EntryPoint.h"

#include "Core/Engine.h"
#include "Platform/RendererBase.h"

ENTRY_APP(Game)

Game::Game()
{
	std::cout << "Construct..." << std::endl;
}

Game::~Game()
{
	printf("Destruc\n");
}

void Game::Initialize()
{
	printf("Init\n");

}

void Game::Shutdown()
{
	printf("Shutfown\n");
}

void Game::Update()
{
	//printf("Update\n");
	LAG::Renderer::SetPipelineState(0);
}
