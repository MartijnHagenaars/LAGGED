#include "GL_LineTool.h"

#include "GL/glew.h"
#include "Core/Resources/Shader.h"
#include "Platform/OpenGL/Renderer/GL_ErrorChecks.h"

#include "Core/Engine.h"
#include "Core/Resources/ResourceManager.h"
#include "ECS/Components/CameraComponent.h"
#include "ECS/Systems/CameraSystem.h"

namespace LAG
{
	LineTool::LineTool()
	{
	}

	LineTool::~LineTool()
	{
	}

	void LineTool::Initialize()
	{
		if (!GetResourceManager()->AddResource<Shader>(HashedString("res/Shaders/OpenGL/LineRender")))
			Logger::Critical("Failed to load line drawing shader.");
		m_LineShader = GetResourceManager()->GetResource<Shader>(HashedString("res/Shaders/OpenGL/LineRender"));

		LAG_GRAPHICS_CHECK(glGenVertexArrays(1, &m_VAO));
		LAG_GRAPHICS_CHECK(glGenBuffers(1, &m_VBO));
		LAG_GRAPHICS_CHECK(glBindVertexArray(m_VAO));

		LAG_GRAPHICS_CHECK(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
		LAG_GRAPHICS_CHECK(glBufferData(GL_ARRAY_BUFFER, sizeof(LinePositions), nullptr, GL_STATIC_DRAW));

		LAG_GRAPHICS_CHECK(glEnableVertexAttribArray(0));
		LAG_GRAPHICS_CHECK(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0));
	}

	void LineTool::Shutdown()
	{
		LAG_GRAPHICS_CHECK(glDeleteVertexArrays(1, &m_VAO));
		LAG_GRAPHICS_CHECK(glDeleteBuffers(1, &m_VBO));
	}

	void LineTool::DrawLine(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& color)
	{
		Entity camEntity = CameraSystem::GetActiveCameraEntity();

		m_LineShader->Bind();
		m_LineShader->SetMat4("a_MVP", CameraSystem::CalculateProjMat(&camEntity) * CameraSystem::CalculateViewMat(&camEntity));
		m_LineShader->SetVec3("a_LineColor", color);

		LAG_GRAPHICS_CHECK(glBindVertexArray(m_VAO));
		LAG_GRAPHICS_CHECK(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
		m_LinePositions.p1 = p1, m_LinePositions.p2 = p2;
		LAG_GRAPHICS_CHECK(glBufferData(GL_ARRAY_BUFFER, sizeof(LinePositions), &m_LinePositions, GL_STATIC_DRAW));

		LAG_GRAPHICS_CHECK(glDrawArrays(GL_LINES, 0, 2));
	}
}

