#pragma once
#include <memory>
#include "Defines.h"

#include "Core/ApplicationInterface.h"
#include "Events/EventBase.h"

namespace LAG
{
	class Window;
	class ResourceManager;
	class Engine
	{
	public:
		LAG_API ~Engine();

		LAG_API int Run(IApplication* applicationPtr);

		ResourceManager* GetResources() const { return m_ResourceManager; }

	private:

		Engine(const Engine&) = delete;
		friend Engine& GetEngine();
		Engine() {}

		bool Initialize(IApplication* applicationPtr);
		bool Shutdown();

		void EventCallback(EventBase& event);

	private:
		LAG::IApplication* m_Application = nullptr;
		Window* m_PrimaryWindow = nullptr;
		ResourceManager* m_ResourceManager = nullptr;
	};

	static Engine& GetEngine()
	{
		static Engine engine;
		return engine;
	}
	static ResourceManager* GetResourceManager() { return GetEngine().GetResources(); }
}
