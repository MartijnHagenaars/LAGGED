#include "Precomp.h"
#include "GL_Renderer.h"

#include "Platform/OpenGL/Window/GL_Window.h"

namespace LAG::Renderer
{
	struct RendererData
	{
		const Window::WindowData* winData = nullptr;

	};
	RendererData* renderData = nullptr;


	bool Initialize()
	{
		if (renderData != nullptr || Window::GetWindowData() == nullptr)
		{
			Utility::Logger::Critical("RenderData / WindowData objects are invalid.");
			return false;
		}
		
		renderData = new RendererData();
		renderData->winData = static_cast<const Window::WindowData*>(Window::GetWindowData());
		

		return true;
	}

	bool Shutdown()
	{
		return false;
	}

	void StartFrame()
	{

	}

	void EndFrame()
	{

	}

	void Render()
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(renderData->winData->window);
	}

	void Clear()
	{

	}
}