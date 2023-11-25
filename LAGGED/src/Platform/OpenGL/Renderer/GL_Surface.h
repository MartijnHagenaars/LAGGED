#pragma once
#include "Core/Resources/Surface.h"

namespace LAG
{
	class Texture;
	class Surface : public SurfaceBase
	{
	public:
		Surface();
		Surface(int xResolution, int zResolution, const std::string& heightTexturePath);
		~Surface() override;
		

		//TODO: Make this a virtual in the SurfaceBase
		void SetTessellationQuality();
		void SetTessellationQuality(int xResolution, int zResolution, const std::string& heightTexturePath);

		void Render(TransformComponent& transform, uint32_t cameraEntityID, Shader& shader) override;

	private:
		virtual bool Load() override;
		virtual bool Unload() override;

		std::vector<float> m_Vertices;
		std::vector<unsigned short> m_Indices;

		unsigned int m_VBO = 0;
		unsigned int m_EBO = 0;
		unsigned int m_VAO = 0;
	};
}