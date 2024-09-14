#pragma once
#include "Core/Defines.h"
#include "Core/ApplicationInterface.h"

class World;
class GameApp : public LAG::IApplication
{
public:
	GameApp() = default;

	void Initialize() override;
	void Shutdown() override;

	void Update() override;

private:
	World* m_World = nullptr;
};

