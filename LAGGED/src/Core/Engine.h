#pragma once
#include <memory>
#include "Defines.h"

#include "Core/ApplicationInterface.h"
#include "Events/EventBase.h"

namespace LAG
{
	class Window;
	class Scene;
	class ResourceManager;
	class WindowManager;
	class Engine
	{
	public:
		Engine(const Engine&) = delete;
		~Engine();

		int Run(IApplication* applicationPtr);

		ResourceManager* GetResources() const { return m_ResourceManager; }
		WindowManager* GetWindowManager() const { return m_WindowManager; }
		Scene* GetScene() const { return m_Scene; }

	private:
		friend Engine& GetEngine();
		Engine()
		{
			std::cout << "Engine constructor" << std::endl;
		}

		bool Initialize(IApplication* applicationPtr);
		bool Shutdown();

		void EventCallback(EventBase& event);

	private:
		LAG::IApplication* m_Application = nullptr;
		WindowManager* m_WindowManager = nullptr;
		ResourceManager* m_ResourceManager = nullptr;
		Scene* m_Scene = nullptr;
	};

	Engine& GetEngine();

	inline ResourceManager* GetResourceManager() { return GetEngine().GetResources(); }
	inline WindowManager* GetWindowManager() { return GetEngine().GetWindowManager(); }
}
