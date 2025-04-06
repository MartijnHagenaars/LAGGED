#include "Platform/RenderBackend.h"

#include "Utility/DebugLine.h"

#include "Resources/Model.h"
#include "Resources/Shader.h"
#include "Resources/Texture.h"

#include "Core/Engine.h"
#include "Core/Resources/ResourceManager.h"

#include "Platform/Window.h"
#include "Platform/Resources/Shader.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace LAG
{
	ErrResult LAG::Initialize()
	{
		//Create some essential shaders.
		GetResourceManager()->AddResource<Shader>(HashedString("res/Shaders/OpenGL/ObjectShader"));
		GetResourceManager()->AddResource<Shader>(HashedString("res/Shaders/OpenGL/SurfaceShader"));

		glEnable(GL_DEPTH_TEST);

		DebugLine::Initialize();

		//Setup resize callback
		GetWindow()->SetResizeCallBack(std::bind(&Renderer::OnResize, this, std::placeholders::_1, std::placeholders::_2));

		return ErrResult::SUCCESS;
	}

	ErrResult LAG::Shutdown()
	{
		DebugLine::Shutdown();

		return ErrResult::SUCCESS;
	}

	void LAG::DrawDebugLine(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& color)
	{
		DebugLine::AddLineToQueue(p1, p2, color);
	}
}