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

		//If any data already exists, clear it.
		if (m_VertexData.capacity() > 0)
		{
			//First, clear the vertex data vector
			m_VertexData.clear();
			m_VertexData.shrink_to_fit();

			//After that, clear indices
			m_Indices.clear();
			m_Indices.shrink_to_fit();
		}


		CalculateVertices();
		CalculateIndices();
		CalculateNormals();

		stbi_image_free(m_HeightMap);
	}

	void Surface::Render(TransformComponent& transform, uint32_t cameraEntityID, Shader& shader)
	{
		shader.Bind();
		shader.SetMat4("a_ModelMat", transform.GetTransformMatrix());
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
		m_Indices.reserve((m_Height - 1) * (m_Width - 1) * 6);

		for (int h = 0; h < m_Height - 1; ++h) {
			for (int w = 0; w < m_Width - 1; ++w) 
			{
				// Calculate indices for two triangles, forming a quad
				unsigned int topLeft = h * m_Width + w;
				unsigned int topRight = topLeft + 1;
				unsigned int bottomLeft = (h + 1) * m_Width + w;
				unsigned int bottomRight = bottomLeft + 1;

				//First triangle
				m_Indices.push_back(topLeft);
				m_Indices.push_back(bottomLeft);
				m_Indices.push_back(topRight);

				//Second triangle
				m_Indices.push_back(topRight);
				m_Indices.push_back(bottomLeft);
				m_Indices.push_back(bottomRight);
			}
		}
	}

	void Surface::CalculateNormals()
	{
		for (unsigned int i = 0; i < m_Indices.size(); i += 3)
		{
			//Get all the indices
			unsigned int index1 = m_Indices[i + 0];
			unsigned int index2 = m_Indices[i + 1];
			unsigned int index3 = m_Indices[i + 2];

			//Get all the vertices
			glm::vec3& v0 = m_VertexData[index1].vertex;
			glm::vec3& v1 = m_VertexData[index2].vertex;
			glm::vec3& v2 = m_VertexData[index3].vertex;

			//Calculate normal
			const glm::vec3 normal = glm::normalize(glm::cross(v1 - v0, v2 - v0));

			//Apply the normal calculations to the vertices
			m_VertexData[index1].normal += normal;
			m_VertexData[index2].normal += normal;
			m_VertexData[index3].normal += normal;
		}

		//Normalize each vertex normal
		for (unsigned int i = 0; i < m_VertexData.size(); i++)
			m_VertexData[i].normal = glm::normalize(m_VertexData[i].normal);

	}
}
