#include "Engine.h"
#include "Platform/Base/ExceptionBase.h"
#include "Utility/Timer.h"

#include "Platform/Window.h"
#include "Resources/ResourceManager.h"
#include "Editor/ToolsManager.h"
#include "Platform/Renderer.h"

#include "Core/Resources/Model.h"

#include "ECS/Scene.h"
#include "ECS/Entity.h"

#include "ECS/Systems/BasicSystems.h"

namespace LAG
{
	Engine::~Engine()
	{
		Shutdown();
	}

	int Engine::Run(IApplication* applicationPtr)
	{
		if (Initialize(applicationPtr) != true)
		{
			Logger::Critical("Failed to initialize.");
			return -1;
		}

		LAG::Timer timer;
		float elapsedTime = 0.f;
		int frames = 0;


		//Main loop
		while (GetWindow()->IsOpen())
		{
			GetWindow()->Update();

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
		Logger::Initialize();



		//Create the window manager and a primary window
		m_Window = new Window(1920, 1280, "LAGGED", false);

		//Create the resource manager
		m_ResourceManager = new ResourceManager();

		//Create the renderer
		m_Renderer = new Renderer();
		m_Renderer->Initialize();

		//Create the scene
		m_Scene = new Scene();

		m_ToolsManager = new ToolsManager();
		m_ToolsManager->Initialize();

		//Add some input actions that'll be used by the engine and the editor. 
		Input::AddInputAction(Input::InputType::LAG_W, HashedString("cameraMoveForward"));
		Input::AddInputAction(Input::InputType::LAG_S, HashedString("cameraMoveBackward"));
		Input::AddInputAction(Input::InputType::LAG_A, HashedString("cameraMoveLeft"));
		Input::AddInputAction(Input::InputType::LAG_D, HashedString("cameraMoveRight"));
		Input::AddInputAction(Input::InputType::LAG_SHIFT_L, HashedString("cameraMoveUp"));
		Input::AddInputAction(Input::InputType::LAG_CONTROL_L, HashedString("cameraMoveDown"));
		Input::AddInputAction(Input::InputType::LAG_UP, HashedString("cameraLookUp"));
		Input::AddInputAction(Input::InputType::LAG_DOWN, HashedString("cameraLookDown"));
		Input::AddInputAction(Input::InputType::LAG_LEFT, HashedString("cameraLookLeft"));
		Input::AddInputAction(Input::InputType::LAG_RIGHT, HashedString("cameraLookRight"));

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

		if (m_ToolsManager != nullptr)
			m_ToolsManager->Shutdown();
		delete m_ToolsManager;

		m_Renderer->Shutdown();
		delete m_Renderer;

		if (m_ResourceManager != nullptr)
			m_ResourceManager->Clear();
		delete m_ResourceManager;
		m_ResourceManager = nullptr;

		m_Window->Shutdown();
		delete m_Window;
		m_Window = nullptr;

		Logger::Shutdown();

		return true;
	}

	Engine& GetEngine()
	{
		static Engine engine;
		return engine;
	}
}