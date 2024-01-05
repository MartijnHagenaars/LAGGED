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
		m_VertexData =
		{
			VertexData{glm::vec3(-0.5f, 0.0f,-0.5f)},
			VertexData{glm::vec3(-0.5f, 0.0f, 0.5f)},
			VertexData{glm::vec3( 0.5f, 0.0f, 0.5f)},
			VertexData{glm::vec3( 0.5f, 0.0f,-0.5f)}
		};

		m_Indices =
		{
			0, 1, 2,
			0, 2, 3
		};
	}

	glm::vec3 CalculateSurfaceNormal(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3) 
	{
		glm::vec3 U = p2 - p1;
		glm::vec3 V = p3 - p1;

		//Should be a cross
		glm::vec3 normal;
		normal.x = (U.y * V.z) - (U.z * V.y);
		normal.y = (U.z * V.x) - (U.x * V.z);
		normal.z = (U.x * V.y) - (U.y * V.x);

		return glm::normalize(normal);
	}

	void Surface::SetTessellationQuality(const std::string& heightTexturePath)
	{
		//TODO: Add some sort of check here to see if the terrain is too big

		//Texture width and height are currently just kind of ignored. This is bad. Some downscaling thing should be added.
		int textureWidth, textureHeight;

		//Check if there is a way of loading without getting the texture width and height. It's not important here.
		m_HeightMap = stbi_load(heightTexturePath.c_str(), &m_TextureWidth, &m_TextureHeight, &m_HeightMapColorChannels, 0);
		if (m_HeightMap == nullptr)
		{
			Logger::Error("Failed to load height map texture data for height map texture with the following path: {0}", heightTexturePath);
			return;
		}

		//Check if parameters are valid
		//TODO: This check is dumb and should be changed. 
		if (m_Width <= 0 || m_Height <= 0)
			return;

		CalculateVertices();
		CalculateIndices();
		//CalculateNormals();

		stbi_image_free(m_HeightMap);
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

		//TOOD: Should use unsigned short
		glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);
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
		LAG_GRAPHICS_EXCEPTION(glBufferData(GL_ARRAY_BUFFER, m_VertexData.size() * sizeof(VertexData), &m_VertexData[0], GL_STATIC_DRAW));
		LAG_GRAPHICS_EXCEPTION(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO));
		LAG_GRAPHICS_EXCEPTION(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned int), &m_Indices[0], GL_STATIC_DRAW));

		LAG_GRAPHICS_EXCEPTION(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0));
		LAG_GRAPHICS_EXCEPTION(glEnableVertexAttribArray(0));
		LAG_GRAPHICS_EXCEPTION(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))));
		LAG_GRAPHICS_EXCEPTION(glEnableVertexAttribArray(1));

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

	void Surface::CalculateVertices()
	{
		//float widthAdjustment = static_cast<float>(m_Width) / textureWidth;
		float widthAdjustment = static_cast<float>(m_TextureWidth) / m_Width;
		float heightAdjustment = static_cast<float>(m_TextureHeight) / m_Height;

		m_VertexData.clear();
		//TODO: m_Height and m_Width should be renamed to m_Column and m_Row
		m_VertexData.reserve(m_Height * m_Width);
		for (unsigned int h = 0; h < m_Height; h++)
		{
			for (unsigned int w = 0; w < m_Width; w++)
			{
				//Calculate vertices
				float xVert = -m_Height * 0.5f + m_Height * h / static_cast<float>(m_Height);
				float zVert = -m_Width * 0.5f + (m_Width * w / static_cast<float>(m_Width));

				unsigned int xResize = static_cast<unsigned int>(widthAdjustment * static_cast<float>(w));
				unsigned int zResize = static_cast<unsigned int>(heightAdjustment * static_cast<float>(h));
				unsigned char* heightPosData = m_HeightMap + ((xResize + m_TextureWidth * zResize) * m_HeightMapColorChannels);
				float yVert = static_cast<int>(heightPosData[0]) * m_YScale - m_YScaleShift;

				VertexData vd;
				vd.vertex = glm::vec3(xVert, yVert, zVert);
				m_VertexData.push_back(vd);
			}
		}
	}

	void Surface::CalculateIndices()
	{
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
					else m_Indices.push_back(static_cast<unsigned int>(indexValue));
				}
			}
		}
	}

	void Surface::CalculateNormals()
	{
		for (size_t i = 0; i < m_Indices.size(); i += 3)
		{
			const auto kIdx1 = m_Indices[i + 0];
			const auto kIdx2 = m_Indices[i + 1];
			const auto kIdx3 = m_Indices[i + 2];

			const auto& v0 = m_VertexData[kIdx1].vertex;
			const auto& v1 = m_VertexData[kIdx2].vertex;
			const auto& v2 = m_VertexData[kIdx3].vertex;

			const glm::vec3 kNormal = glm::normalize(
				glm::cross(v1 - v0, v2 - v0)
			);

			// Save for each vertex
			m_VertexData[kIdx1].normal += kNormal;
			m_VertexData[kIdx2].normal += kNormal;
			m_VertexData[kIdx3].normal += kNormal;
		}

		for (size_t i = 0; i < m_VertexData.size(); ++i)
		{
			//Normalizing 0 0 0 results in nan values? wtf
			m_VertexData[i].normal = glm::normalize(m_VertexData[i].normal);
		}
	}
}
