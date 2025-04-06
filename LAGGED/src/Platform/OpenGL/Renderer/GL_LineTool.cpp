#include "GL_LineTool.h"

#include "GL/glew.h"
#include "Platform/Resources/Shader.h"
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

	}

	void LineTool::Shutdown()
	{
		LAG_GRAPHICS_CHECK(glDeleteVertexArrays(1, &m_VAO));
		LAG_GRAPHICS_CHECK(glDeleteBuffers(1, &m_VBO));
	}

	void LineTool::DrawLine(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& color)
	{
		EntityID camEntityID = CameraSystem::GetActiveCameraEntityID();

		m_LineShader->Bind();
		m_LineShader->SetMat4("a_MVP", CameraSystem::CalculateProjMat(camEntityID) * CameraSystem::CalculateViewMat(camEntityID));
		m_LineShader->SetVec3("a_LineColor", color);

		LAG_GRAPHICS_CHECK(glBindVertexArray(m_VAO));
		LAG_GRAPHICS_CHECK(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
		m_LinePositions.p1 = p1, m_LinePositions.p2 = p2;
		LAG_GRAPHICS_CHECK(glBufferData(GL_ARRAY_BUFFER, sizeof(LinePositions), &m_LinePositions, GL_STATIC_DRAW));

		LAG_GRAPHICS_CHECK(glDrawArrays(GL_LINES, 0, 2));
	}
}

