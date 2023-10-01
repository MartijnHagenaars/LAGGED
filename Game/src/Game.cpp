#include "Game.h"
#include <iostream>
#include "Core/EntryPoint.h"

#include "Core/Engine.h"
#include "Platform/Base/Renderer/RendererBase.h"

#include "Core/Memory/ResourcePtr.h"

ENTRY_APP(Game)

Game::Game()
{
	std::cout << "Construct..." << std::endl;
}

Game::~Game()
{
	printf("Destruct\n");
}

//ResourcePtr test struct
struct CoolThing
{
	CoolThing(std::string name, int health) :
		m_Name(name), m_Health(health)
	{}

	float m_Slider = 1.f;
	bool m_Button = true;
	int m_Health = 100;
	std::string m_Name = "john";
};

//ResourcePtr test function 1
void RefFunc(LAG::ResourcePtr<CoolThing>& thingy)
{
	thingy->m_Slider += 1.f;
}

//ResourcePtr test function 2
void CopyFunc(LAG::ResourcePtr<CoolThing> thingy)
{
	thingy->m_Slider += 2.f;
}

//ResourcePtr test function 3
void RemoveRefFunc(LAG::ResourcePtr<CoolThing>& thingy)
{
	thingy.Release();
	printf("Has object been deleted (in ref func)? %s\n", (thingy.Get() == nullptr) ? "true" : "false");
}

//ResourcePtr test function 4
void RemoveCopyFunc(LAG::ResourcePtr<CoolThing> thingy)
{
	thingy.Release();
	printf("Has object been deleted (in copy func)? %s\n", (thingy.Get() == nullptr) ? "true" : "false");
}

void Game::Initialize()
{
	LAG::Input::AddInputAction(LAG::Input::InputType::LAG_W, LAG::Utility::String("UpButton"));
	LAG::Input::AddInputAction(LAG::Input::InputType::LAG_S, LAG::Utility::String("DownButton"));
	LAG::Input::AddInputAction(LAG::Input::InputType::LAG_LMB, LAG::Utility::String("LMB"));

	////ObserverPtr test 1:
	//{
	//	LAG::ResourcePtr<CoolThing> coolObject = LAG::ResourcePtr<CoolThing>("Frank", 200);
	//	printf("slider value 1 (start): %f\n", coolObject.Get()->m_Slider);
	//	RefFunc(coolObject);
	//	printf("slider value 2 (ref): %f\n", coolObject.Get()->m_Slider);
	//	CopyFunc(coolObject);
	//	printf("slider value 3 (copy): %f\n", coolObject.Get()->m_Slider);
	//	RefFunc(coolObject);
	//	printf("slider value 4 (ref): %f\n", coolObject.Get()->m_Slider);
	//	CopyFunc(coolObject);
	//	printf("slider value 5 (copy): %f\n", coolObject.Get()->m_Slider);

	//	RemoveCopyFunc(coolObject);
	//	printf("Has object been deleted? %s\n", (coolObject.Get() == nullptr) ? "true" : "false");
	//}

	////ObserverPtr test 2:
	//{
	//	CoolThing* coolObject = new CoolThing("Francis", 400);
	//	LAG::ResourcePtr<CoolThing> resource(coolObject);
	//	printf("slider value 1 (start): %f\n", resource.Get()->m_Slider);
	//	RefFunc(resource);
	//	printf("slider value 2 (ref): %f\n", resource.Get()->m_Slider);
	//	CopyFunc(coolObject);
	//	printf("slider value 3 (copy): %f\n", resource.Get()->m_Slider);
	//	RefFunc(resource);
	//	printf("slider value 4 (ref): %f\n", resource.Get()->m_Slider);

	//	//RemoveCopyFunc(coolObject);
	//	RemoveCopyFunc(resource);
	//	printf("Has object been deleted? %s\n", (resource.Get() == nullptr) ? "true" : "false");
	//}

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
	else if (LAG::Input::IsActionPressedOnce(LAG::Utility::String("LMB")))
	{
		float x, y;
		LAG::Input::GetMousePosition(x, y);
		LAG::Utility::Logger::Info("Clickity at {0}, {1}!\n", x, y);
	}
}
