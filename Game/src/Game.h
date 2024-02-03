#pragma once
#include "Core/Defines.h"
#include "Core/ApplicationInterface.h"

class Game : public LAG::IApplication
{
public:
	Game();
	~Game() override;

	void Initialize() override;
	void Shutdown() override;

	void Update() override;
};

