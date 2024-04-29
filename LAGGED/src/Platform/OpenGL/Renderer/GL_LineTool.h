#pragma once
#include "glm/vec3.hpp"

namespace LAG
{
	struct LinePositions
	{
		glm::vec3 p1 = glm::vec3(0.f);
		glm::vec3 p2 = glm::vec3(0.f);
	};

	class Shader;
	class LineTool
	{
	public:
		static void DrawLine(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& color);

		static void Initialize();
		static void Shutdown();
		
	private:
		LineTool();
		~LineTool();
		LineTool(const LineTool& other) = delete;
		LineTool& operator=(LineTool& other) = delete;

		inline static unsigned int m_VBO;
		inline static unsigned int m_VAO;
		inline static Shader* m_LineShader;
		inline static LinePositions m_LinePositions;
	};
}

