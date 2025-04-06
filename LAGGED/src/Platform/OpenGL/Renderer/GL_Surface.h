#pragma once
#include "glm/vec3.hpp"

#include "Core/Resources/Shader.h"
#include "ECS/Scene.h"
#include "ECS/Components/BasicComponents.h"
#include "ECS/Components/LightComponent.h"

#include "Platform/OpenGL/Renderer/GL_Buffers.h"

namespace LAG
{
	struct ProceduralSurfaceComponent;

	class Texture;
	class Surface
	{
	public:		
		//Load a height map from a texture
		//TODO: Needs to be reworked
		void GenerateHeightMapSurface(const std::string& heightTexturePath);
		void GenerateNoiseSurface(const TransformComponent& transformComp, const ProceduralSurfaceComponent& procSurfaceComp);

		virtual bool Load();
		virtual bool Unload();

		void Render(EntityID objectEntity, EntityID cameraEntity, Shader& shader, std::vector<std::pair<TransformComponent*, LightComponent*>>& lights);

	private:
		////Creates a vector of 2D noise data
		//std::vector<float> GenerateNoise(int xStart, int yStart, int xSize, int ySize, float frequency, int seed = 0);

		//Generates the surface. Calculates vertices, indices and normals.
		void GenerateSurface(int width, int height);

		void CalculateVertices();
		void CalculateIndices();
		void CalculateNormals();

		bool m_Loaded = false;

		struct VertexData
		{
			glm::vec3 vertex = glm::vec3(0.f);
			glm::vec3 normal = glm::vec3(0.f);
		};

		std::vector<VertexData> m_VertexData;
		std::vector<unsigned int> m_Indices;

		int m_Width = 0;
		int m_Height = 0;
		int m_Subdivisions = 0;

		std::vector<float> m_HeightMapData;
		int m_HeightMapWidth = 0;
		int m_HeightMapHeight = 0;

		ArrayBuffer m_Buffer;
	};
}