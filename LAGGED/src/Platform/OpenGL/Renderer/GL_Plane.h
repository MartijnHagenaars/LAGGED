#pragma once
#include "Core/Resources/Plane.h"

namespace LAG
{
	class Plane : public PlaneBase
	{
	public:
		Plane() : PlaneBase(Utility::String("")) {}
		explicit Plane(const Utility::String& path);
		~Plane() override;

		void Render(TransformComponent& transform, uint32_t cameraEntityID, Shader& shader) override;

	private:
		virtual bool Load() override;
		virtual bool Unload() override;

		unsigned int m_VBO = 0;
		unsigned int m_EBO = 0;
		unsigned int m_VAO = 0;
	};
}