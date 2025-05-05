#pragma once
#include <glm/vec3.hpp>
#include <vector>

namespace LAG
{
	struct LinePositions
	{
		glm::vec3 p1 = glm::vec3(0.f);
		glm::vec3 p2 = glm::vec3(0.f);
	};

	class Shader;
	class DebugLine
	{
	public:
		static void Initialize();
		static void Shutdown();

		static void AddLineToQueue(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& color);

		static void PresentDebugLines();

	private:
		inline static unsigned int m_VBO;
		inline static unsigned int m_VAO;
		inline static Shader* m_LineShader;
		inline static LinePositions m_LinePositions;

		struct LineData
		{
			glm::vec3 pos1, pos2, color;
		};
		inline static std::vector<LineData> m_LineRenderList;
	};
}

