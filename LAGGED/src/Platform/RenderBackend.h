#pragma once
#include "Core/Defines.h"
#include "Utility/Timer.h"
#include <glm/vec3.hpp>

namespace LAG
{
	class Model;
	class Shader;
	class Texture;
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

	private:
		void OnResize(unsigned int width, unsigned int height);

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