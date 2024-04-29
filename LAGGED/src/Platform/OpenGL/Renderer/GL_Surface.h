#pragma once
#include "Core/Resources/Surface.h"
#include "glm/vec3.hpp"

namespace LAG
{
	struct ProceduralSurfaceComponent;

	enum class SurfaceType
	{
		FLAT = 0,

	};

	class Texture;
	class Surface : public SurfaceBase
	{
	public:
		Surface();
		Surface(const std::string& heightTexturePath);
		
		//Load a height map from a texture
		//TODO: Needs to be reworked
		void GenerateHeightMapSurface(const std::string& heightTexturePath);
		void GenerateNoiseSurface(const TransformComponent& transformComp, const ProceduralSurfaceComponent& procSurfaceComp);

		virtual bool Load() override;
		virtual bool Unload() override;


		void Render(TransformComponent& transform, Entity* cameraEntity, Shader& shader, std::vector<std::pair<TransformComponent*, LightComponent*>>& lights) override;
	private:
		////Creates a vector of 2D noise data
		//std::vector<float> GenerateNoise(int xStart, int yStart, int xSize, int ySize, float frequency, int seed = 0);

		//Generates the surface. Calculates vertices, indices and normals.
		void GenerateSurface(int width, int height);


		void CalculateVertices();
		void CalculateIndices();
		void CalculateNormals();

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

	};
}