#pragma once
#include "Core/Resources/Model.h"
#include "glm/glm.hpp" //TODO: Remove glm here

namespace tinygltf
{
	class Model;
	class Node;
	struct Mesh;
}

namespace LAG
{
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

		void LoadModelNode(tinygltf::Model& model, tinygltf::Node& node);
		void LoadMesh(tinygltf::Model& model, tinygltf::Mesh& mesh);

		void RenderModelNode(tinygltf::Model& model, tinygltf::Node& node);
		void RenderModelMesh(tinygltf::Model& model, tinygltf::Mesh& mesh);

		unsigned int m_VBO = 0;
		unsigned int m_EBO = 0;
		unsigned int m_VAO = 0;

	};
}
