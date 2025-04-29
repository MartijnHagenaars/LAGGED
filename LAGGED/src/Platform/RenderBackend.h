#pragma once
#include "Core/Defines.h"
#include "Utility/Timer.h"
#include <glm/vec3.hpp>

namespace LAG
{
	class Model;
	class Shader;
	class Texture;
	class Surface;
	class Renderer
	{
	public:
		static ErrResult Initialize();
		static ErrResult Shutdown();

		static void PresentFrame();
		static void DrawDebugLine(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& color);

		static Model* GetModel();
		static Shader* GetShader();
		static Texture* GetTexture();
		static Surface* GetSurface();

		// TODO: This is currently public, which shouldn't be the case.
		// But this is currently necessary to get the window resizing to work. 
		// In the future, windows (be it the app window or an editor window) should trigger this themselves.
		static void OnResize(unsigned int width, unsigned int height);

	private:

		struct RenderProperties
		{
			bool useLighting = true;
			bool showWireframe = false;
		};
		static RenderProperties m_Properties;

		//Variables for calculating the time it takes to render
		static inline LAG::Timer m_RenderTimer;
		static inline float m_RenderTime = 0.f;
	};
}