#pragma once
#include "Core/Resources/Model.h"
#include "glm/glm.hpp"

namespace tinygltf
{
	class Model;
}

namespace LAG
{
	class Model : public ModelBase
	{
	public:
		Model() = delete;
		explicit Model(const Utility::String& path);
		~Model() override;

		void Render() override;

	private:
		bool Load() override;
		bool Unload() override;

		void LoadTextures(const tinygltf::Model& modelData, const std::string& directoryPath) override;
		void LoadMeshes(const tinygltf::Model& modelData, const std::string& directoryPath) override;

		unsigned int m_VBO = 0;
		unsigned int m_EBO = 0;
		unsigned int m_VAO = 0;

	};
}
