#include "Precomp.h"
#include "Engine.h"
#include "Platform/Base/ExceptionBase.h"

#include "Utility/Timer.h"
#include "Utility/Logger.h"

#include "Platform/Base/Window/WindowBase.h"
#include "Platform/Base/Window/WindowManager.h"
#include "Platform/Base/Renderer/RendererBase.h"

#include "Resources/ResourceManager.h"
#include "Core/Resources/Model.h"

//using LAG::Utility

namespace LAG
{
	Engine::~Engine()
	{
		Shutdown(); 
	}

	int Engine::Run(IApplication* applicationPtr)
	{
		try
		{
			Utility::Logger::Initialize();

			//Create primary window
			m_PrimaryWindow = WindowManager::Get().AddWindow(800, 600, "Main window!", false);
			if (m_PrimaryWindow == nullptr)
				LAG_ASSERT("Primary window was nullptr.");

			if (Initialize(applicationPtr) != true)
			{
				Utility::Logger::Critical("Failed to initialize.");
				return -1;
			}

			LAG::Utility::Timer timer;
			float elapsedTime = 0.f;
			int frames = 0;

			//Main loop
			while (WindowManager::Get().AreWindowsOpen())
			{
				WindowManager::Get().Update();

				m_Application->Update();
				//Renderer::Render();

				//Framerate counter: 
				++frames;
				elapsedTime += timer.Mark();
				if (elapsedTime >= 18.f)
				{
					Utility::Logger::Info("FPS: {0}", frames / elapsedTime);
					elapsedTime = 0.f;
					frames = 0;
				}
			}

			return 0;
		}
		catch (ExceptionBase& e)
		{
			Utility::Logger::Critical(e.GetExceptionMessage().c_str());
			system("pause");
			return -3; 
		}
		catch (std::exception& e)
		{
			Utility::Logger::Critical("Standard exception thrown: {0}", e.what());
			system("pause");
			return -2; 
		}
		catch (...)
		{
			Utility::Logger::Critical("Unknown exception thrown: no information available.");
			system("pause");
			return -1;
		}

	}

	bool Engine::Initialize(IApplication* applicationPtr)
	{
		//Setup window

		m_PrimaryWindow->SetWindowEventCallback(std::bind(&Engine::EventCallback, this, std::placeholders::_1));

		//Setup renderer
		if (!Renderer::Initialize())
		{
			LAG::Utility::Logger::Critical("Failed to initialize renderer.");
			return false;
		}

		m_ResourceManager = new ResourceManager();

		//Application setup. Should be the last object to be initialized!
		m_Application = applicationPtr;
		m_Application->Initialize();

		return true;
	}

	bool Engine::Shutdown()
	{
		if (m_Application != nullptr)
		{
			m_Application->Shutdown();
			delete m_Application;
		}
		m_Application = nullptr;

		if (m_ResourceManager != nullptr)
			delete m_ResourceManager;
		m_ResourceManager = nullptr;



		Renderer::Shutdown();
		WindowManager::Get().Shutdown();

		Utility::Logger::Shutdown(); //TODO: Logger shutdown should happen after every other shutdowns. Fix the crash first. 

		return true;
	}

	void Engine::EventCallback(EventBase& event)
	{
		std::cout << "Event callback detected.\n";
	}

	Engine& GetEngine()
	{
		static Engine engine;
		return engine;
	}
}