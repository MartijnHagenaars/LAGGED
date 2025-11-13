#pragma once
#include "Core/Defines.h"

#include <glm/vec3.hpp>
#include <functional>
#include <vector>

#include "Utility/Profiler.h"
#include "Utility/Hash.h"
#include "Utility/Timer.h"

namespace LAG
{
	class Skybox;
	class Renderer
	{
	public:
		static ErrResult Initialize();
		static ErrResult Shutdown();

		static void PresentFrame();

		static void SetSkyboxCubemap(const StringHash64& path);
		static void DrawDebugLine(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& color);

		// TODO: This is currently public, which shouldn't be the case.
		// But this is currently necessary to get the window resizing to work. 
		// In the future, windows (be it the app window or an editor window) should trigger this themselves.
		static void OnResize(unsigned int width, unsigned int height);

		static void RegisterImGuiRenderCallback(std::function<void()> func);

	private:
		struct RenderProperties
		{
			bool useLighting = true;
			bool showWireframe = false;
		};
		inline static RenderProperties m_Properties;

		//Variables for calculating the time it takes to render
		static inline LAG::Timer m_RenderTimer;
		static inline float m_RenderTime = 0.f;

		static inline Skybox* m_Skybox = nullptr;

		static inline std::vector<std::function<void()>> m_ImGuiRenderCallbacks;
	};
}