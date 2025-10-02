#include "Engine.h"
#include "Utility/Timer.h"

#include "Resources/ResourceManager.h"
#include "Editor/ToolsManager.h"

#include "Platform/Window.h"
#include "Platform/RenderBackend.h"

#include "ECS/Scene.h"
#include "ECS/Entity.h"

#include "IO/FileWatch.h"

#include "ECS/Systems/BasicSystems.h"

namespace LAG
{
	Engine& GetEngine()
	{
		static Engine engine;
		return engine;
	}

	Engine::~Engine()
	{
		Shutdown();
	}

	int Engine::Run(IApplication* applicationPtr)
	{
		m_EngineInitTime = std::chrono::steady_clock::now();
		if (Initialize(applicationPtr) != true)
		{
			CRITICAL("Failed to initialize.");
			return -1;
		}

		LAG::Timer timer;
		float elapsedTime = 0.f;
		int frames = 0;


		//Main loop
		while (GetWindow()->IsOpen())
		{
			GetWindow()->Update();

			FileWatch::CheckWatches();

			//Update all basic systems
			BasicSystems::UpdateBasicSystems();

			m_Application->Update();

			//TODO: Do I move this somewhere else?
			GetWindow()->PresentFrame();

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

	bool Engine::Initialize(IApplication* applicationPtr)
	{
		INFO("Initializing engine...");

		//Create the window manager and a primary window
		m_Window = new Window(1280, 720, "LAGGED", false, false);

		//Create the resource manager
		m_ResourceManager = new ResourceManager();

		//Create the renderer
		m_Renderer = new Renderer();
		m_Renderer->Initialize();

		//Create the scene
		m_Scene = new Scene();

		m_ToolsManager = new ToolsManager();
		m_ToolsManager->Initialize();
		m_Renderer->RegisterImGuiRenderCallback(std::bind(&ToolsManager::PresentEditor, m_ToolsManager));

		//Add some input actions that'll be used by the engine and the editor. 
		Input::AddInputAction(Input::InputType::LAG_W, StringHash64("cameraMoveForward"));
		Input::AddInputAction(Input::InputType::LAG_S, StringHash64("cameraMoveBackward"));
		Input::AddInputAction(Input::InputType::LAG_A, StringHash64("cameraMoveLeft"));
		Input::AddInputAction(Input::InputType::LAG_D, StringHash64("cameraMoveRight"));
		Input::AddInputAction(Input::InputType::LAG_SHIFT_L, StringHash64("cameraMoveUp"));
		Input::AddInputAction(Input::InputType::LAG_CONTROL_L, StringHash64("cameraMoveDown"));
		Input::AddInputAction(Input::InputType::LAG_UP, StringHash64("cameraLookUp"));
		Input::AddInputAction(Input::InputType::LAG_DOWN, StringHash64("cameraLookDown"));
		Input::AddInputAction(Input::InputType::LAG_LEFT, StringHash64("cameraLookLeft"));
		Input::AddInputAction(Input::InputType::LAG_RIGHT, StringHash64("cameraLookRight"));

		//Application setup. Should be the last object to be initialized!
		m_Application = applicationPtr;
		m_Application->Initialize();

		return true;
	}

	bool Engine::Shutdown()
	{
		INFO("Shutting down engine...");

		if (m_Application)
		{
			m_Application->Shutdown();
			delete m_Application;
		}
		m_Application = nullptr;

		if (m_ToolsManager)
		{
			m_ToolsManager->Shutdown();
			delete m_ToolsManager;
		}
		m_ToolsManager = nullptr;

		if(m_Scene)
			delete m_Scene;
		m_Scene = nullptr;

		if (m_ResourceManager != nullptr)
		{
			m_ResourceManager->Clear();
			delete m_ResourceManager;
		}
		m_ResourceManager = nullptr;

		if (m_Renderer)
		{
			m_Renderer->Shutdown();
			delete m_Renderer;
		}
		m_Renderer = nullptr;

		m_Window->Shutdown();
		delete m_Window;
		m_Window = nullptr;

		//Logger::Shutdown();

		return true;
	}

	long long Engine::GetElapsedTimeSinceInit()
	{
		long long elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - m_EngineInitTime).count();  // Calculate elapsed time in milliseconds
		return elapsedMs;
	}
}