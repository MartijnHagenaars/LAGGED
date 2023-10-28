#pragma once
#include "Core/Resources/Resource.h"
#include "ECS/Components/BasicComponents.h"

namespace tinygltf
{
	class Model;
}

namespace LAG
{
	struct LightData
	{
		glm::vec3 m_LightPos = glm::vec3(0.f);
		glm::vec3 m_LightColor = glm::vec3(1.f);
		float m_LightIntensity = 1.f;
	};

	class Shader;
	class ModelBase : public Resource
	{
	public:
		ModelBase() = delete;
		explicit ModelBase(const Utility::String& path) : Resource(path) {};
		virtual ~ModelBase() {};

		virtual void Render(TransformComponent& transform, Shader& shader, std::array<LightData, 3> lights) = 0;

	protected:
		tinygltf::Model* m_Model = nullptr;
		float m_PreTransformScale = 1.f;

	private:
		virtual bool Load() = 0;
		virtual bool Unload() = 0;

		virtual void LoadTextures(tinygltf::Model& modelData, std::string& directoryPath) = 0;
		virtual void LoadModel(tinygltf::Model& modelData, std::string& directoryPath) = 0;
	};
}

//Include the correct model header, based on the project configuration
#ifdef PLATFORM_OPENGL
#include "Platform/OpenGL/Renderer/GL_Model.h"
#elif PLATFORM_DIRECTX
#include "Platform/DX12/Renderer/DX12_Model.h"
#endif
