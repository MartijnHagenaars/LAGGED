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

//For debugging only. 
#include "ImGui/imgui.h"

namespace LAG
{
	Surface::Surface() : 
		SurfaceBase()
	{
		SetTessellationQuality();
	}

	Surface::Surface(const std::string& heightTexturePath) :
		SurfaceBase()
	{
		int texWidth, texHeight, colorChannels;
		stbi_info(heightTexturePath.c_str(), &texWidth, &texHeight, &colorChannels);
		m_TextureWidth = texWidth, m_TextureHeight = texHeight;

		m_Width = m_TextureWidth, m_Height = m_TextureHeight;
		m_EditorWidth = m_Width, m_EditorHeight = m_Height;

		SetTessellationQuality(heightTexturePath);
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

	void Surface::SetTessellationQuality(const std::string& heightTexturePath)
	{
		//TODO: Add some sort of check here to see if the terrain is too big

		int colorChannels = 0;
		//Texture width and height are currently just kind of ignored. This is bad. Some downscaling thing should be added.
		int textureWidth, textureHeight;

		//Check if there is a way of loading without getting the texture width and height. It's not important here.
		unsigned char* heightMapData = stbi_load(heightTexturePath.c_str(), &m_TextureWidth, &m_TextureHeight, &colorChannels, 0);
		if (heightMapData == nullptr)
		{
			Logger::Error("Failed to load height map texture data for height map texture with the following path: {0}", heightTexturePath);
			return;
		}

		//Check if parameters are valid
		//TODO: This check is dumb and should be changed. 
		if (m_Width <= 0 || m_Height <= 0)
			return;

		//float widthAdjustment = static_cast<float>(m_Width) / textureWidth;
		float widthAdjustment = static_cast<float>(m_TextureWidth) / m_Width;
		float heightAdjustment = static_cast<float>(m_TextureHeight) / m_Height;

		m_Vertices.clear();
		m_Vertices.reserve(m_Height * m_Width * 3);
		for (unsigned int z = 0; z < m_Height; z++)
		{
			for (unsigned int x = 0; x < m_Width; x++)
			{
				//Calculate vertices
				float xVert = -m_Height * 0.5f + m_Height * z / static_cast<float>(m_Height);
				float zVert = -m_Width * 0.5f + (m_Width * x / static_cast<float>(m_Width));

				unsigned int xResize = static_cast<unsigned int>(widthAdjustment * static_cast<float>(x));
				unsigned int zResize = static_cast<unsigned int>(heightAdjustment * static_cast<float>(z));
				unsigned char* heightPosData = heightMapData + ((xResize + m_TextureWidth * zResize) * colorChannels);
				float yVert = static_cast<int>(heightPosData[0]) * m_YScale - m_YScaleShift;

				m_Vertices.insert(m_Vertices.end(), { xVert, yVert, zVert });
				
				//Next thing to do here is calculate the normals
				//... TODO!
			}
		}

		m_Indices.clear();
		m_Indices.reserve((m_Height - 1) * m_Width * 2);
		for (unsigned int z = 0; z < m_Height - 1; z += 1)
		{
			for (unsigned int x = 0; x < m_Width; x += 1)
			{
				for (unsigned int stripSide = 0; stripSide < 2; stripSide++)
				{
					unsigned int indexValue = x + m_Width * (z + stripSide);
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
		const int totalStrips = (m_Height - 1) / resolution;
		const int trisPerStrip = (m_Width / resolution) * 2 - 2;
		for (unsigned int strip = 0; strip < totalStrips; strip++)
		{
			glDrawElements(GL_TRIANGLE_STRIP, trisPerStrip + 2, GL_UNSIGNED_SHORT, (void*)(sizeof(unsigned short) * (trisPerStrip + 2) * strip));
		}
	}

	void Surface::DrawDebugWindow()
	{
		ImGui::Begin("Surface Properties");

		ImGui::SliderInt("Width", &m_EditorWidth, 0, 1280);
		ImGui::SliderInt("Height", &m_EditorHeight, 0, 1280);
		ImGui::SliderFloat("Y-Scale", &m_YScale, 0.f, 1.f);
		ImGui::SliderFloat("Y-Scale Shift", &m_YScaleShift, 0.f, 512.f);

		if (ImGui::Button("Apply"))
		{
			m_Width = m_EditorWidth, m_Height = m_EditorHeight;
			SetTessellationQuality("res/Assets/Textures/face.png");
			Reload();
		}


		ImGui::End();
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
		LAG_GRAPHICS_EXCEPTION(glDeleteVertexArrays(1, &m_VAO));

		return true;
	}
}
