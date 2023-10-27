#pragma once
#include <memory>
#include "Defines.h"

#include "Core/ApplicationInterface.h"
#include "Events/EventBase.h"

namespace LAG
{
	class Window;
	class Scene;
	class Engine
	{
	public:
		LAG_API ~Engine();

		LAG_API int Run(IApplication* applicationPtr);

		LAG_API Scene* GetScene() const { return m_Scene; }

	private:

		Engine(const Engine&) = delete;
		friend Engine& GetEngine();
		Engine() {}

		bool Initialize(IApplication* applicationPtr);
		bool Shutdown();

		void EventCallback(EventBase& event);

	private:
		std::unique_ptr<LAG::IApplication> m_App;
		Window* m_PrimaryWindow = nullptr;

		Scene* m_Scene = nullptr;
	};

	static Engine& GetEngine()
	{
		static Engine engine;
		return engine;
	}
}
