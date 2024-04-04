#include "GL_LineTool.h"
#include "Core/Resources/Shader.h"
#include "GL/glew.h"

#include "Core/Engine.h"
#include "Core/Resources/ResourceManager.h"
#include "ECS/Components/CameraComponent.h"
#include "ECS/Systems/CameraSystem.h"

namespace LAG
{
	LineTool::LineTool()
	{
		if (!GetResourceManager()->AddResource<Shader>(HashedString("res/Shaders/OpenGL/FrameBuffer")))
			Logger::Critical("Failed to load line drawing shader.");
		m_LineShader = GetResourceManager()->GetResource<Shader>(HashedString("res/Shaders/OpenGL/FrameBuffer"));

		glGenVertexArrays(1, &m_VAO);
		glGenBuffers(1, &m_VBO);
		glBindVertexArray(m_VAO);

		//glBindBuffer(GL_ARRAY_BUFFER, VBO);
		//glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), &line, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	LineTool::~LineTool()
	{
		glDeleteVertexArrays(1, &m_VAO);
		glDeleteBuffers(1, &m_VBO);
	}

	void LineTool::DrawLine(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& color)
	{
		Entity camEntity = CameraSystem::GetActiveCameraEntity();

		m_LineShader->Bind();

		m_LineShader->SetMat4("a_MVP", CameraSystem::CalculateProjMat(&camEntity) * CameraSystem::CalculateViewMat(&camEntity));
		m_LineShader->SetVec3("a_LineColor", color);

		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		m_LinePositions.p1 = p1, m_LinePositions.p2 = p2;
		glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), &m_LinePositions, GL_STATIC_DRAW);

		glBindVertexArray(m_VAO);
		glDrawArrays(GL_LINES, 0, 2);
	}
}

