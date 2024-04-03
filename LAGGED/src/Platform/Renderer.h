#pragma once
#include "Core/Defines.h"

namespace LAG
{
	class Renderer
	{
	public:
		Renderer();
		~Renderer();

		void Render();

		void DrawLine(const glm::vec3& p1, const glm::vec3& p2);

		void OnResize(unsigned int width, unsigned int height);

		void SetPipelineState(int pipelineID);

	private:
		bool Initialize();
		bool Shutdown();

		/// <summary>
		/// Start of the ImGui frame
		/// </summary>
		void ImGuiFrameStart();

		/// <summary>
		/// End of the ImGui frame
		/// </summary>
		void ImGuiFrameEnd();
	};

}
