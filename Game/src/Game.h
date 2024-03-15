#pragma once
#include "Core/Defines.h"
#include "Core/ApplicationInterface.h"

class World;
class Game : public LAG::IApplication
{
public:
	Game();
	~Game() override;

	void Initialize() override;
	void Shutdown() override;

	void Update() override;

private:
	World* m_World = nullptr;
};

