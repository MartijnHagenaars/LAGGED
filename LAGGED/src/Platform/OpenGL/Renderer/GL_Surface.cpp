#include "GL_Surface.h"
#include "Utility/Logger.h"

#include "Core/Resources/Shader.h"
#include "Core/Resources/Texture.h"

#include "ECS/Components/BasicComponents.h"
#include "ECS/Components/CameraComponent.h"
#include "ECS/Components/LightComponent.h"

#include "ECS/Systems/CameraSystem.h"

#include "GL/glew.h"
#include "Exceptions/GL_GraphicsExceptionMacros.h"
#include <glm/ext/matrix_transform.hpp>

#include "stb_image.h"


#include "FastNoise/FastNoise.h"

//For debugging only. 
#include "ImGui/imgui.h"

namespace LAG
{
	Surface::Surface() :
		SurfaceBase()
	{
	}

	Surface::Surface(const std::string& heightTexturePath) :
		SurfaceBase()
	{
		Logger::Error("Constructor for loading height map from texture has not been implemented.");
	}

	Surface::~Surface()
	{
	}

	
	//////////////////////////////////////////
	// Loading height map data from texture //
	//////////////////////////////////////////

	void Surface::LoadTextureHeightMap(const std::string& heightTexturePath)
	{
		Logger::Error("LoadTextureHeightMap() has not been implemented.");
	}


	////////////////////////////////////////
	// Loading height map data from noise //
	////////////////////////////////////////

	void Surface::GenerateSurface(int xStart, int yStart, int xSize, int ySize, float frequency, float amplitude, int seed)
	{
		//Check if sizes are valid
		m_Width = xSize, m_Height = ySize;
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

		m_TextureWidth = m_Width, m_TextureHeight = m_Height;
		m_Amplitude = amplitude;

		m_HeightMapData = GenerateNoise(xStart, yStart, xSize, ySize, frequency, seed);

		CalculateVertices();
		CalculateIndices();
		CalculateNormals();
	}

	std::vector<float> Surface::GenerateNoise(int xStart, int yStart, int xSize, int ySize, float frequency, int seed)
	{
		std::vector<float> noiseData(xSize * ySize);

		if (xSize > 0 && ySize > 0)
		{
			//TODO: generator should be stored
			auto generator = FastNoise::NewFromEncodedNodeTree("GQANAAIAAAC4HoU+BwAAuB4lQACF61E/ASQAAgAAABwAARkADQACAAAAuB4FQP//AAAAj8I1QQB7FK4+AQ0AAgAAAMP1qED//wAAAJqZmT4AcT0KPwAAAABA");
			generator->GenUniformGrid2D(noiseData.data(), xStart, yStart, xSize, ySize, frequency, seed);
		}
		else
			Logger::Warning("Invalid noise map size");

		return noiseData;
	}

	void Surface::Render(TransformComponent& transform, Entity* cameraEntity, Shader& shader, std::vector<std::pair<TransformComponent*, LightComponent*>>& lights)
	{
		shader.Bind();
		shader.SetMat4("a_ModelMat", transform.GetTransformMatrix());
		shader.SetMat4("a_ViewMat", CameraSystem::CalculateViewMat(cameraEntity));
		shader.SetMat4("a_ProjMat", CameraSystem::CalculateProjMat(cameraEntity));

		//TODO: The way lights are currently handled isn't that good and should be revisited in the future.
		if (lights.size() > 0)
		{
			shader.SetBool("a_UseLight", true);
			for (int i = 0; i < TOTAL_POINT_LIGHTS; i++)
			{
				if (i < lights.size())
				{
					shader.SetVec3(std::string("a_PointLightData[" + std::to_string(i) + "].a_LightPosition"), lights[i].first->GetPosition());
					shader.SetVec3("a_PointLightData[" + std::to_string(i) + "].a_LightColor", lights[i].second->lightColor);
					shader.SetFloat("a_PointLightData[" + std::to_string(i) + "].a_LightIntensity", lights[i].second->lightIntensity);
					shader.SetFloat("a_PointLightData[" + std::to_string(i) + "].a_LightAttenuation", lights[i].second->lightAttenuation);
				}
			}

		}
		else shader.SetBool("a_UseLight", false);

		LAG_GRAPHICS_EXCEPTION(glBindVertexArray(m_VAO));

		//TOOD: Should use unsigned short
		glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);
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
				float yVert = m_HeightMapData[xResize + m_TextureWidth * zResize] * m_Amplitude - m_YScaleShift;

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
			m_VertexData[i].normal = glm::normalize(-m_VertexData[i].normal);

	}
}
