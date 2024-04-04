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
		void DrawLine(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& color);

	private:
		LineTool();
		~LineTool();
		LineTool(const LineTool& other) = delete;
		LineTool& operator=(LineTool& other) = delete;

		unsigned int m_VBO, m_VAO;
		Shader* m_LineShader;
		LinePositions m_LinePositions;
	};
}

