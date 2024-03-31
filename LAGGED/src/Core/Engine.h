#pragma once
#include <memory>
#include "Defines.h"

#include "Core/ApplicationInterface.h"

namespace LAG
{
	class Window;
	class Scene;
	class ResourceManager;
	class Engine
	{
	public:
		Engine(const Engine&) = delete;
		~Engine();

		int Run(IApplication* applicationPtr);

		Window* GetWindow() const { return m_Window; }
		ResourceManager* GetResources() const { return m_ResourceManager; }
		Scene* GetScene() const { return m_Scene; }
		float GetFPS() const { return m_FPS; }
		float GetDeltaTime() const { return m_DeltaTime; }

	private:
		friend Engine& GetEngine();
		Engine() {}

		bool Initialize(IApplication* applicationPtr);
		bool Shutdown();

	private:
		IApplication* m_Application = nullptr;
		
		Window* m_Window = nullptr;
		Scene* m_Scene = nullptr;
		ResourceManager* m_ResourceManager = nullptr;

		float m_FPS = 0.f;
		float m_DeltaTime = 0.f;
	};

	Engine& GetEngine();

	inline Window* GetWindow() { return GetEngine().GetWindow(); }
	inline ResourceManager* GetResourceManager() { return GetEngine().GetResources(); }
	inline Scene* GetScene() { return GetEngine().GetScene(); }
}
