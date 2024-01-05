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
		

		//TODO: Make this a virtual in the SurfaceBase
		void SetTessellationQuality();
		void SetTessellationQuality(const std::string& heightTexturePath);

		void Render(TransformComponent& transform, uint32_t cameraEntityID, Shader& shader) override;

		void DrawDebugWindow();

	private:
		virtual bool Load() override;
		virtual bool Unload() override;

		void CalculateVertices();
		void CalculateIndices();
		void CalculateNormals();

		struct VertexData
		{
			glm::vec3 vertex;
			glm::vec3 normal = glm::vec3(0.f);
		};

		std::vector<VertexData> m_VertexData;
		//std::vector<float> m_Normals;
		std::vector<unsigned short> m_Indices;

		unsigned int m_VBO = 0;
		unsigned int m_EBO = 0;
		unsigned int m_VAO = 0;

		int m_Width = 0;
		int m_Height = 0;

		int m_HeightMapColorChannels;
		unsigned char* m_HeightMap;

		int m_TextureWidth = 0;
		int m_TextureHeight = 0;

		int m_EditorWidth = 0;
		int m_EditorHeight = 0;

		float m_Resolution = 0;
		float m_YScale = 0.25f;
		float m_YScaleShift = 16.f;

	};
}