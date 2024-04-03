#pragma once
#include "Core/Defines.h"
#include "Utility/Timer.h"
#include "glm/vec3.hpp"

namespace LAG
{
	class Renderer
	{
	public:
		Renderer();
		~Renderer();
		Renderer(const Renderer& other) = delete;
		Renderer& operator=(const Renderer& other) = delete;

		/// <summary>
		/// Initialize the renderer. Should only be called once.
		/// </summary>
		/// <returns>Returns true if the renderer initialized correctly.</returns>
		bool Initialize();

		/// <summary>
		/// Shuts down the renderer. Should only be called once.
		/// </summary>
		/// <returns>Returns true if the renderer has shut down correctly.</returns>
		bool Shutdown();

		void Render();

		//TODO: IMPLEMENT!
		void DrawLine(const glm::vec3& p1, const glm::vec3& p2);

		void OnResize(unsigned int width, unsigned int height);

		void SetPipelineState(int pipelineID);

	private:
		/// <summary>
		/// Start of the ImGui frame
		/// </summary>
		void ImGuiFrameStart();

		/// <summary>
		/// End of the ImGui frame
		/// </summary>
		void ImGuiFrameEnd();

		//TODO: THIS SHOULD BE TURNED INTO A TOOL.
		void DrawOptionsWindow();

		//Options used by the renderer
		bool m_ShowWireframe = false;
		bool m_UseLighting = true;

		//Variables for calculating the time it takes to render
		LAG::Timer m_RenderTimer;
		float m_RenderTime = 0.f;
	};

}
