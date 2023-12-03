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

	Surface::Surface(float xResolution, float zResolution, const std::string& heightTexturePath) :
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
		   -0.5f, 0.0f,	-0.5f,
		   -0.5f, 0.0f,	 0.5f,
			0.5f, 0.0f,	 0.5f,
			0.5f, 0.0f,	-0.5f
		};

		m_Indices =
		{
			0, 1, 2,
			0, 2, 3
		};
	}

	void Surface::SetTessellationQuality(float xResolution, float zResolution, const std::string& heightTexturePath)
	{
		//Check if parameters are valid
		if (xResolution <= 0 || zResolution <= 0)
			return;

		m_XVertexResolution = xResolution, m_ZVertexResolution = zResolution;

		int colorChannels = 0;
		//Texture width and height are currently just kind of ignored. This is bad. Some downscaling thing should be added.
		int textureWidth, textureHeight;
		unsigned char* heightMapData = stbi_load(heightTexturePath.c_str(), &textureWidth, &textureHeight, &colorChannels, 0);
		if (heightMapData == nullptr)
		{
			Logger::Error("Failed to load height map texture data for height map texture with the following path: {0}", heightTexturePath);
			return;
		}

		float yScale = 64.0f / 256.0f, yShift = 0.f;

		m_Vertices.clear();
		m_Vertices.reserve(m_ZVertexResolution * m_XVertexResolution * 3);
		for (unsigned int z = 0; z < m_ZVertexResolution; z++)
		{
			for (unsigned int x = 0; x < m_XVertexResolution; x++)
			{
				//Calculate vertices
				float xVert = -m_ZVertexResolution * 0.5f + m_ZVertexResolution * z / static_cast<float>(m_ZVertexResolution);
				float zVert = -m_XVertexResolution * 0.5f + m_XVertexResolution * x / static_cast<float>(m_XVertexResolution);

				unsigned char* heightPosData = heightMapData + ((x + m_XVertexResolution * z) * colorChannels);
				float yVert = static_cast<int>(heightPosData[0]) * yScale - yShift;

				m_Vertices.insert(m_Vertices.end(), { xVert, yVert, zVert });
				
				//Next thing to do here is calculate the normals
				//... TODO!
			}
		}

		m_Indices.clear();
		m_Indices.reserve((m_ZVertexResolution - 1) * m_XVertexResolution * 2);
		for (unsigned int z = 0; z < m_ZVertexResolution - 1; z += 1)
		{
			for (unsigned int x = 0; x < m_XVertexResolution; x += 1)
			{
				for (unsigned int stripSide = 0; stripSide < 2; stripSide++)
				{
					unsigned int indexValue = x + m_XVertexResolution * (z + stripSide);
					if (indexValue > USHRT_MAX)
					{
						//TODO: Better error checking here!
						//This check should also be done before building the terrain and all the vertices
						printf("This terrain type is too big!");

						return;
					}
					else m_Indices.push_back(static_cast<unsigned short>(indexValue));
				}
			}
		}

		stbi_image_free(heightMapData);
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
		shader.SetMat4("a_ViewMat", CameraSystem::CalculateViewMat(cameraEntityID));
		shader.SetMat4("a_ProjMat", CameraSystem::CalculateProjMat(cameraEntityID));

		LAG_GRAPHICS_EXCEPTION(glBindVertexArray(m_VAO));

		int resolution = 1;
		const int totalStrips = (m_ZVertexResolution - 1) / resolution;
		const int trisPerStrip = (m_XVertexResolution / resolution) * 2 - 2;
		for (unsigned int strip = 0; strip < totalStrips; strip++)
		{
			glDrawElements(GL_TRIANGLE_STRIP, trisPerStrip + 2, GL_UNSIGNED_SHORT, (void*)(sizeof(unsigned short) * (trisPerStrip + 2) * strip));
		}
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

		LAG_GRAPHICS_EXCEPTION(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0));
		LAG_GRAPHICS_EXCEPTION(glEnableVertexAttribArray(0));

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
