#include "GL_DebugLine.h"
#include "GL_ErrorChecks.h"

#include <GL/glew.h>

#include "Core/Engine.h"
#include "Core/Resources/ResourceManager.h"

#include "ECS/Components/CameraComponent.h"
#include "ECS/Systems/CameraSystem.h"

#include "Platform/Resources/Shader.h"

namespace LAG
{
	void DebugLine::Initialize()
	{
		if (!GetResourceManager()->AddResource<Shader>(StringHash64("res/Shaders/OpenGL/LineRender")))
			CRITICAL("Failed to load line drawing shader.");
		m_LineShader = GetResourceManager()->GetResource<Shader>(StringHash64("res/Shaders/OpenGL/LineRender"));

		LAG_GRAPHICS_CHECK(glGenVertexArrays(1, &m_VAO));
		LAG_GRAPHICS_CHECK(glGenBuffers(1, &m_VBO));
		LAG_GRAPHICS_CHECK(glBindVertexArray(m_VAO));

		LAG_GRAPHICS_CHECK(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
		LAG_GRAPHICS_CHECK(glBufferData(GL_ARRAY_BUFFER, sizeof(LinePositions), nullptr, GL_STATIC_DRAW));

		LAG_GRAPHICS_CHECK(glEnableVertexAttribArray(0));
		LAG_GRAPHICS_CHECK(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0));
	}

	void DebugLine::Shutdown()
	{
		LAG_GRAPHICS_CHECK(glDeleteVertexArrays(1, &m_VAO));
		LAG_GRAPHICS_CHECK(glDeleteBuffers(1, &m_VBO));
	}

	void DebugLine::AddLineToQueue(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& color)
	{
		m_LineRenderList.emplace_back(LineData{ p1, p2, color });
	}

	void DebugLine::PresentDebugLines()
	{
		EntityID camEntityID = CameraSystem::GetActiveCameraEntityID();

		m_LineShader->Bind();
		m_LineShader->SetMat4("a_MVP", CameraSystem::CalculateProjMat(camEntityID) * CameraSystem::CalculateViewMat(camEntityID));

		for (const auto& it : m_LineRenderList)
		{
			m_LineShader->SetVec3("a_LineColor", it.color);

			LAG_GRAPHICS_CHECK(glBindVertexArray(m_VAO));
			LAG_GRAPHICS_CHECK(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
			m_LinePositions.p1 = it.pos1, m_LinePositions.p2 = it.pos2;
			LAG_GRAPHICS_CHECK(glBufferData(GL_ARRAY_BUFFER, sizeof(LinePositions), &m_LinePositions, GL_STATIC_DRAW));

			LAG_GRAPHICS_CHECK(glDrawArrays(GL_LINES, 0, 2));
		}
		m_LineRenderList.clear();
	}
}