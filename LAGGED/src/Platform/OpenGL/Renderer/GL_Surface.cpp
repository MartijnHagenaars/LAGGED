#include "GL_Surface.h"
#include "Utility/Logger.h"

#include "Core/Resources/Shader.h"
#include "Core/Resources/Texture.h"

#include "ECS/Components/BasicComponents.h"
#include "ECS/Components/CameraComponent.h"
#include "ECS/Components/LightComponent.h"
#include "ECS/Components/TerrainComponents.h"

#include "ECS/Systems/CameraSystem.h"

#include "Utility/Noise.h"

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

	void Surface::GenerateSurface(int width, int height)
	{
		//Check if sizes are valid
		m_Width = width, m_Height = height;
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

		//Clear data that we no longer need
		m_HeightMapData.clear();
		m_HeightMapData.shrink_to_fit();
	}

	//////////////////////////////////////////
	// Loading height map data from texture //
	//////////////////////////////////////////
	void Surface::GenerateHeightMapSurface(const std::string& heightTexturePath)
	{
		Logger::Critical("GenerateHeightMapSurface() has not been implemented.");
	}

	////////////////////////////////////////
	// Loading height map data from noise //
	////////////////////////////////////////
	void Surface::GenerateNoiseSurface(const TransformComponent& transformComp, const ProceduralSurfaceComponent& procSurfaceComp)
	{
		//Set noise positions
		glm::vec2 noisePos = procSurfaceComp.noiseProperties.m_UseTransformPositionForNoise ?
			glm::vec2(transformComp.GetPosition().x, transformComp.GetPosition().z) :
			procSurfaceComp.noiseProperties.m_NoisePosition;

		glm::vec2 noiseScale = glm::vec2(static_cast<float>(procSurfaceComp.surfaceSubdivisions));

		m_Subdivisions = procSurfaceComp.surfaceSubdivisions;
		Noise::GeneratedData noiseData = Noise::GenerateNoiseData(procSurfaceComp.noiseProperties, noisePos, noiseScale, m_Subdivisions);
		m_HeightMapData = noiseData.m_Data;
		m_HeightMapWidth = static_cast<int>(noiseData.m_Size.x);
		m_HeightMapHeight = static_cast<int>(noiseData.m_Size.y);

		GenerateSurface(procSurfaceComp.surfaceSubdivisions, procSurfaceComp.surfaceSubdivisions);
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

		m_Buffer.Render();
	}

	bool Surface::Load()
	{
		if (IsLoaded())
		{
			Logger::Warning("Tried to load a surface thats's already loaded");
			return false;
		}

		VertexBuffer vertexBuffer;
		vertexBuffer.SetVertexData(&m_VertexData.data()[0], m_VertexData.size() * sizeof(VertexData));
		vertexBuffer.SetLayout(std::vector<BufferLayoutElement>{
			BufferLayoutElement("a_Position", BufferVariableType::Float3, false),
			BufferLayoutElement("a_Normal", BufferVariableType::Float3, false)
		});

		IndexBuffer indexBuffer;
		indexBuffer.SetIndexData(m_Indices);

		m_Buffer.Initialize(vertexBuffer, indexBuffer);

		SetLoaded(true);
		return true;
	}

	bool Surface::Unload()
	{
		if (!IsLoaded())
			return true;

		m_Buffer.Shutdown();

		SetLoaded(false);
		return true;
	}

	void Surface::CalculateVertices()
	{
		float widthAdjustment = static_cast<float>(m_HeightMapWidth) / m_Width;
		float heightAdjustment = static_cast<float>(m_HeightMapHeight) / m_Height;

		//TODO: m_Height and m_Width should be renamed to m_Column and m_Row
		m_VertexData.reserve(m_Subdivisions * m_Subdivisions);
		for (int h = 0; h < m_Subdivisions; h++)
		{
			for (int w = 0; w < m_Subdivisions; w++)
			{
				//Calculate vertices
				float xVert = h / static_cast<float>(m_Subdivisions - 1);
				float zVert = w / static_cast<float>(m_Subdivisions - 1);

				float yVert = 0;
				if (!m_HeightMapData.empty())
				{
					unsigned int xResize = static_cast<unsigned int>(std::round(widthAdjustment * static_cast<float>(w)));
					unsigned int zResize = static_cast<unsigned int>(std::round(heightAdjustment * static_cast<float>(h)));
					yVert = m_HeightMapData[zResize + m_HeightMapWidth * xResize];
				}

				VertexData vd;
				vd.vertex = glm::vec3(xVert, yVert, zVert);
				m_VertexData.push_back(vd);
			}
		}
	}

	void Surface::CalculateIndices()
	{
		m_Indices.reserve((m_Subdivisions - 1) * (m_Subdivisions - 1) * 6);

		for (int h = 0; h < m_Subdivisions - 1; ++h) {
			for (int w = 0; w < m_Subdivisions - 1; ++w)
			{
				// Calculate indices for two triangles, forming a quad
				unsigned int topLeft = h * m_Subdivisions + w;
				unsigned int topRight = topLeft + 1;
				unsigned int bottomLeft = (h + 1) * m_Subdivisions + w;
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
