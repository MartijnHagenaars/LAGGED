#include "Precomp.h"
#include "GL_Surface.h"

#include "Core/Resources/Shader.h"
#include "Core/Resources/Texture.h"

#include "ECS/Components/BasicComponents.h"
#include "ECS/Components/CameraComponent.h"

#include "ECS/Systems/CameraSystem.h"

#include "GL/glew.h"
#include "Exceptions/GL_GraphicsExceptionMacros.h"
#include <glm/ext/matrix_transform.hpp>

#include "stb_image.h"

namespace LAG
{
	Surface::Surface() : 
		SurfaceBase()
	{
		SetTessellationQuality();
	}

	Surface::Surface(int xResolution, int zResolution, const std::string& heightTexturePath) :
		SurfaceBase()
	{
		SetTessellationQuality(xResolution, zResolution, heightTexturePath);
	}

	Surface::~Surface()
	{
	}

	void Surface::SetTessellationQuality()
	{
		m_Vertices =
		{
		   -0.5f, -0.5f,  0.0f,		0.0f,  0.0f,  1.0f,		0.0f, 0.0f,  //Bottom-left
		   -0.5f,  0.5f,  0.0f,		0.0f,  0.0f,  1.0f,		0.0f, 1.0f,  //Top-left
			0.5f,  0.5f,  0.0f,		0.0f,  0.0f,  1.0f,		1.0f, 1.0f,  //Top-right
			0.5f, -0.5f,  0.0f,		0.0f,  0.0f,  1.0f,		1.0f, 0.0f   //Bottom-right
		};

		m_Indices =
		{
			0, 1, 2,
			0, 2, 3
		};
	}

	void Surface::SetTessellationQuality(int xResolution, int zResolution, const std::string& heightTexturePath)
	{
		//Check if parameters are valid
		if (xResolution <= 0 || zResolution <= 0)
			return;

		//m_XVertexResolution = xResolution, m_ZVertexResolution = zResolution;

		int colorChannels = 0;
		unsigned char* data = stbi_load(heightTexturePath.c_str(), &m_XVertexResolution, &m_ZVertexResolution, &colorChannels, 0);

		std::vector<glm::vec3> vertices;
		//for (int z = 0; z < zResolution; z++)
		//{
		//	//CONTINUE HERE: https://learnopengl.com/Guest-Articles/2021/Tessellation/Height-map
		//	//heightTexture.
		//	for (int x = 0; x < xResolution; x++)
		//	{

		//	}
		//}

		float yScale = 64.0f / 256.0f, yShift = 16.0f;  // apply a scale+shift to the height data
		for (unsigned int z = 0; z < m_ZVertexResolution; z++)
		{
			for (unsigned int x = 0; x < m_XVertexResolution; x++)
			{
				// retrieve texel for (i,j) tex coord
				unsigned char* texel = data + (x + m_XVertexResolution * z) * colorChannels;
				// raw height at coordinate
				unsigned char y = texel[0];

				// vertex
				vertices.push_back(glm::vec3(-m_ZVertexResolution / 2.0f + z, (int)y * yScale - yShift, -m_XVertexResolution / 2.0f + x));
				//vertices.push_back();        // v.x
				//vertices.push_back(); // v.y
				//vertices.push_back(-width / 2.0f + j / );        // v.z
			}
		}

		stbi_image_free(data);
	}

	void Surface::Render(TransformComponent& transform, uint32_t cameraEntityID, Shader& shader)
	{

		glm::mat4 modelMat = glm::mat4(1.f);
		modelMat = glm::translate(modelMat, transform.position);
		modelMat = glm::rotate(modelMat, transform.rotation.x, glm::vec3(1.f, 0.f, 0.f));
		modelMat = glm::rotate(modelMat, transform.rotation.y, glm::vec3(0.f, 1.f, 0.f));
		modelMat = glm::rotate(modelMat, transform.rotation.z, glm::vec3(0.f, 0.f, 1.f));
		modelMat = glm::scale(modelMat, transform.scale);

		shader.Bind();
		shader.SetMat4("a_ModelMat", modelMat);
		shader.SetMat4("a_ProjMat", CameraSystem::CalculateProjMat(cameraEntityID));
		shader.SetMat4("a_ViewMat", CameraSystem::CalculateViewMat(cameraEntityID));

		LAG_GRAPHICS_EXCEPTION(glBindVertexArray(m_VAO));
		LAG_GRAPHICS_EXCEPTION(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0));
	}

	bool Surface::Load()
	{
		LAG_GRAPHICS_EXCEPTION(glGenVertexArrays(1, &m_VAO));
		LAG_GRAPHICS_EXCEPTION(glGenBuffers(1, &m_VBO));
		LAG_GRAPHICS_EXCEPTION(glGenBuffers(1, &m_EBO));

		LAG_GRAPHICS_EXCEPTION(glBindVertexArray(m_VAO));

		LAG_GRAPHICS_EXCEPTION(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
		LAG_GRAPHICS_EXCEPTION(glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(float), &m_Vertices[0], GL_STATIC_DRAW));
		LAG_GRAPHICS_EXCEPTION(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO));
		LAG_GRAPHICS_EXCEPTION(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned short), &m_Indices[0], GL_STATIC_DRAW));

		LAG_GRAPHICS_EXCEPTION(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0));
		LAG_GRAPHICS_EXCEPTION(glEnableVertexAttribArray(0));
		LAG_GRAPHICS_EXCEPTION(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))));
		LAG_GRAPHICS_EXCEPTION(glEnableVertexAttribArray(1));
		LAG_GRAPHICS_EXCEPTION(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))));
		LAG_GRAPHICS_EXCEPTION(glEnableVertexAttribArray(2));

		LAG_GRAPHICS_EXCEPTION(glBindVertexArray(0));

		return true;
	}

	bool Surface::Unload()
	{
		LAG_GRAPHICS_EXCEPTION(glBindVertexArray(0));
		LAG_GRAPHICS_EXCEPTION(glBindBuffer(GL_ARRAY_BUFFER, 0));
		LAG_GRAPHICS_EXCEPTION(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

		LAG_GRAPHICS_EXCEPTION(glDeleteBuffers(1, &m_VBO));
		LAG_GRAPHICS_EXCEPTION(glDeleteBuffers(1, &m_EBO));
		LAG_GRAPHICS_EXCEPTION(glDeleteBuffers(1, &m_VAO));

		return true;
	}
}
