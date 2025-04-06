#pragma once
#include "Core/Defines.h"
#include "Utility/Timer.h"
#include "glm/vec3.hpp"
#include <vector>

namespace LAG
{
	class Renderer
	{
	public:
		void Render();

		void DrawLine(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& color);

		void OnResize(unsigned int width, unsigned int height);

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

		struct LineData
		{
			glm::vec3 pos1, pos2, color;
		};
		std::vector<LineData> m_LineRenderList;
	};

}
