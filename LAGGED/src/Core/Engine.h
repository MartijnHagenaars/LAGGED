#pragma once
#include <chrono>
#include "Defines.h"

#include "Core/ApplicationInterface.h"

namespace LAG
{
	class Window;
	class Renderer;
	class Scene;
	class ResourceManager;
	class Engine
	{
	public:
		Engine(const Engine&) = delete;
		~Engine();

		int Run(IApplication* applicationPtr);

		Window* GetWindow() const { return m_Window; }
		Renderer* GetRenderer() { return m_Renderer; }
		ResourceManager* GetResources() const { return m_ResourceManager; }
		Scene* GetScene() const { return m_Scene; }

		float GetFPS() const { return m_FPS; }
		float GetDeltaTime() const { return m_DeltaTime; }

		/// <summary>
		/// Returns the elapsed time since engine initialization. Time is stored in milliseconds.
		/// </summary>
		/// <returns>Milliseconds since engine initialization.</returns>
		long long GetElapsedTimeSinceInit();

	private:
		friend Engine& GetEngine();
		Engine() {}

		bool Initialize(IApplication* applicationPtr);
		bool Shutdown();

	private:
		IApplication* m_Application = nullptr;
		
		Window* m_Window = nullptr;
		Renderer* m_Renderer = nullptr;
		Scene* m_Scene = nullptr;
		ResourceManager* m_ResourceManager = nullptr;

		float m_FPS = 0.f;
		float m_DeltaTime = 0.f;

		// Stores the time when the engine initialize function was called. 
		// Used for calculating the elapsed time since engine start.
		std::chrono::steady_clock::time_point m_EngineInitTime;
	};

	Engine& GetEngine();

	inline Window* GetWindow() { return GetEngine().GetWindow(); }
	inline Renderer* GetRenderer() { return GetEngine().GetRenderer(); }
	inline ResourceManager* GetResourceManager() { return GetEngine().GetResources(); }
	inline Scene* GetScene() { return GetEngine().GetScene(); }
}
