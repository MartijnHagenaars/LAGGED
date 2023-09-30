#pragma once
#include "Core/Resources/Model.h"
#include "glm/glm.hpp" //TODO: Remove glm here
#include <map>
#include <utility>

namespace tinygltf
{
	class Model;
	class Node;
	struct Mesh;
}

typedef unsigned int GLuint;

namespace LAG
{
	class Mesh
	{

	};

	class Model : public ModelBase
	{
		friend class ResourceManager;
	public:
		Model() = delete;
		explicit Model(const Utility::String& path);
		~Model() override;

		void Render(Shader& shader) override;

	private:
		bool Load() override;
		bool Unload() override;

		void LoadTextures(tinygltf::Model& modelData, std::string& directoryPath) override;
		void LoadModel(tinygltf::Model& modelData, std::string& directoryPath) override;
		
		std::vector<size_t> m_Textures;
	};
}
