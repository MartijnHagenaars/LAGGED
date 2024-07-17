#pragma once
#include "Core/Defines.h"
#include "Core/ApplicationInterface.h"


class ToolsManager;
class Editor : public LAG::IApplication
{
public:
	Editor() = default;
	~Editor() override = default;

	void Initialize() override;
	void Shutdown() override;

	void Update() override;

private:

};

