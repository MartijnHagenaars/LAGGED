#pragma once
#include <memory>
#include "Defines.h"

#include "Core/ApplicationInterface.h"

namespace LAG
{
	class Window;
	class Renderer;
	class Scene;
	class ToolsManager;
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
		ToolsManager* GetToolsManager() const { return m_ToolsManager; }

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
		Renderer* m_Renderer = nullptr;
		Scene* m_Scene = nullptr;
		ResourceManager* m_ResourceManager = nullptr;
		ToolsManager* m_ToolsManager = nullptr;

		float m_FPS = 0.f;
		float m_DeltaTime = 0.f;
	};

	Engine& GetEngine();

	inline Window* GetWindow() { return GetEngine().GetWindow(); }
	inline Renderer* GetRenderer() { return GetEngine().GetRenderer(); }
	inline ResourceManager* GetResourceManager() { return GetEngine().GetResources(); }
	inline Scene* GetScene() { return GetEngine().GetScene(); }
	inline ToolsManager* GetToolsManager() { return GetEngine().GetToolsManager(); }
}
