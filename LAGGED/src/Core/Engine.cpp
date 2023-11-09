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

#include "ECS/Scene.h"
#include "ECS/Entity.h"

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
			if (Initialize(applicationPtr) != true)
			{
				Utility::Logger::Critical("Failed to initialize.");
				return -1;
			}

			LAG::Utility::Timer timer;
			float elapsedTime = 0.f;
			int frames = 0;

			
			//Main loop
			while (m_WindowManager->AreWindowsOpen())
			{
				m_WindowManager->Update();

				m_Application->Update();

				//Calculate the framerate
				m_DeltaTime = timer.MarkSeconds();
				elapsedTime += m_DeltaTime;
				if (frames++ >= 64)
				{
					m_FPS = static_cast<float>(frames) / (elapsedTime);
					elapsedTime = 0.f, frames = 0;
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
		Utility::Logger::Initialize();

		//Create the window manager and a primary window
		m_WindowManager = new WindowManager();
		Window* newWindow = m_WindowManager->AddWindow(800, 600, "Main window!", false);
		newWindow->SetWindowEventCallback(std::bind(&Engine::EventCallback, this, std::placeholders::_1));

		//Setup renderer
		if (!Renderer::Initialize())
		{
			LAG::Utility::Logger::Critical("Failed to initialize renderer.");
			return false;
		}

		m_ResourceManager = new ResourceManager();
		m_Scene = new Scene();

		//Add some input actions that'll be used by the engine and the editor. 
		Input::AddInputAction(Input::InputType::LAG_W, Utility::String("cameraMoveForward"));
		Input::AddInputAction(Input::InputType::LAG_S, Utility::String("cameraMoveBackward"));
		Input::AddInputAction(Input::InputType::LAG_A, Utility::String("cameraMoveLeft"));
		Input::AddInputAction(Input::InputType::LAG_D, Utility::String("cameraMoveRight"));
		Input::AddInputAction(Input::InputType::LAG_SHIFT_L, Utility::String("cameraMoveUp"));
		Input::AddInputAction(Input::InputType::LAG_CONTROL_L, Utility::String("cameraMoveDown"));
		Input::AddInputAction(Input::InputType::LAG_UP, Utility::String("cameraLookUp"));
		Input::AddInputAction(Input::InputType::LAG_DOWN, Utility::String("cameraLookDown"));
		Input::AddInputAction(Input::InputType::LAG_LEFT, Utility::String("cameraLookLeft"));
		Input::AddInputAction(Input::InputType::LAG_RIGHT, Utility::String("cameraLookRight"));

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

		if (m_WindowManager != nullptr)
		{
			m_WindowManager->Shutdown();
			delete m_WindowManager;
		}
		m_WindowManager = nullptr;


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