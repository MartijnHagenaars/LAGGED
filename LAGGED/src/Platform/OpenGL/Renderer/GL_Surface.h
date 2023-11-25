#pragma once
#include "Core/Resources/Surface.h"

namespace LAG
{
	class Texture;
	class Surface : public SurfaceBase
	{
	public:
		Surface();
		~Surface() override;
		
		//TODO: Make this a virtual in the SurfaceBase
		//TODO: Not a great function name... Maybe come up with something better
		void SetTessellation(int xResolution, int zResolution, Texture& heightTexture);

		void Render(TransformComponent& transform, uint32_t cameraEntityID, Shader& shader) override;

	private:
		virtual bool Load() override;
		virtual bool Unload() override;

		unsigned int m_VBO = 0;
		unsigned int m_EBO = 0;
		unsigned int m_VAO = 0;
	};
}