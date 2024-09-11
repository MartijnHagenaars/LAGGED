#include "GL_Skybox.h"

#include <vector>
#include <glm/vec3.hpp>

namespace LAG
{
	void Skybox::Load()
	{
		std::vector<glm::vec3> vertices =
		{
			{-1.0f, -1.0f, -1.0f},
			{ 1.0f, -1.0f, -1.0f},
			{ 1.0f,  1.0f, -1.0f},
			{-1.0f,  1.0f, -1.0f},
			{-1.0f, -1.0f,  1.0f},
			{ 1.0f, -1.0f,  1.0f},
			{ 1.0f,  1.0f,  1.0f},
			{-1.0f,  1.0f,  1.0f}
		};
		
		std::vector<unsigned int> indices =
		{
			0, 1, 2, 0, 2, 3,
			4, 6, 5, 4, 7, 6,
			4, 0, 3, 4, 3, 7,
			1, 5, 6, 1, 6, 2,
			3, 2, 6, 3, 6, 7,
			4, 5, 1, 4, 1, 0
		};

		VertexBuffer vb;
		vb.SetLayout(BufferLayout({ BufferLayoutElement("Position", BufferVariableType::Int3, false)}));
		vb.SetVertexData(&vertices.data()[0], vertices.size() * sizeof(glm::vec3));

		IndexBuffer ib;
		ib.SetIndexData(indices);
		m_Buffer.Initialize(vb, ib);
	}

	void Skybox::Unload()
	{
		m_Buffer.Shutdown();
	}

	void Skybox::Render(Cubemap& cubemap)
	{
		cubemap.Bind();
		m_Buffer.Render();
	}

}