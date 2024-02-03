#pragma once
#include "Core/Resources/Surface.h"
#include "glm/vec3.hpp"

namespace LAG
{
	class Texture;
	class Surface : public SurfaceBase
	{
	public:
		Surface();
		Surface(const std::string& heightTexturePath);
		~Surface() override;
		
		void GenerateSurface(int xStart, int yStart, int xSize, int ySize, float frequency, float amplitude, int seed = 0);

		void Render(TransformComponent& transform, Entity* cameraEntity, Shader& shader, std::vector<std::pair<TransformComponent*, LightComponent*>>& lights) override;

		void DrawDebugWindow();

	private:
		virtual bool Load() override;
		virtual bool Unload() override;

		//Load a height map from a texture
		//TODO: Needs to be reworked
		void LoadTextureHeightMap(const std::string& heightTexturePath);

		//Creates a vector of 2D noise data
		std::vector<float> GenerateNoise(int xStart, int yStart, int xSize, int ySize, float frequency, int seed = 0);

		void CalculateVertices();
		void CalculateIndices();
		void CalculateNormals();

		struct VertexData
		{
			glm::vec3 vertex = glm::vec3(0.f);
			glm::vec3 normal = glm::vec3(0.f);
		};

		std::vector<VertexData> m_VertexData;
		//std::vector<float> m_Normals;
		std::vector<unsigned int> m_Indices;

		unsigned int m_VBO = 0;
		unsigned int m_EBO = 0;
		unsigned int m_VAO = 0;

		int m_Width = 0;
		int m_Height = 0;

		std::vector<float> m_HeightMapData;

		int m_TextureWidth = 0;
		int m_TextureHeight = 0;

		int m_EditorWidth = 0;
		int m_EditorHeight = 0;

		float m_Amplitude = 0.25f;
		float m_YScaleShift = 16.f;

	};
}