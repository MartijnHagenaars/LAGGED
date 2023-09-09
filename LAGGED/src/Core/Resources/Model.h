#pragma once
#include "Core/Resources/Resource.h"
#include "glm/glm.hpp"

namespace tinygltf
{
	class Model;
}

namespace LAG
{
	class ModelBase : public Resource
	{
	public:
		ModelBase() = delete;
		explicit ModelBase(const Utility::String& path) : Resource(path) {};
		virtual ~ModelBase() {};

		virtual void Render() = 0;

		void SetPosition(glm::vec3& position) { m_Position = position; }
		void SetRotation(glm::vec3& rotation) { m_Rotation = rotation; }
		void SetScale(glm::vec3& scale) { m_Scale = scale; }

		const glm::vec3& GetPosition() const { return m_Position; }
		const glm::vec3& GetRotation() const { return m_Rotation; }
		const glm::vec3& GetScale() const { return m_Scale; }

	protected:
		glm::vec3 m_Position = glm::vec3(0.f);
		glm::vec3 m_Rotation = glm::vec3(0.f);
		glm::vec3 m_Scale = glm::vec3(0.f);
		float m_PreTransformScale = 1.f;

	private:
		virtual bool Load() = 0;
		virtual bool Unload() = 0;

		virtual void LoadTextures(const tinygltf::Model& modelData, const std::string& directoryPath) = 0;
		virtual void LoadModel(const tinygltf::Model& modelData, const std::string& directoryPath) = 0;

	};
}

//Include the correct model header, based on the project configuration
#ifdef PLATFORM_OPENGL
#include "Platform/OpenGL/Renderer/GL_Model.h"
#elif PLATFORM_DIRECTX
#include "Platform/DX12/Renderer/DX12_Model.h"
#endif
