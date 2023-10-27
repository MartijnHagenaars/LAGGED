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
		LAG_API Engine(const Engine&) = delete;
		LAG_API ~Engine();

		LAG_API int Run(IApplication* applicationPtr);

		ResourceManager* GetResources() const { return m_ResourceManager; }

	private:
		friend Engine& GetEngine();
		LAG_API Engine()
		{
			std::cout << "ENgine constructor" << std::endl;
		}

		bool Initialize(IApplication* applicationPtr);
		bool Shutdown();

		void EventCallback(EventBase& event);

	private:
		LAG::IApplication* m_Application = nullptr;
		Window* m_PrimaryWindow = nullptr;
		ResourceManager* m_ResourceManager = nullptr;
	};

	Engine& GetEngine();

	inline ResourceManager* GetResourceManager() { return GetEngine().GetResources(); }
}
