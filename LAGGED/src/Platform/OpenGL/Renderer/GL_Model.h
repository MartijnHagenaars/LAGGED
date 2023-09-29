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

		void LoadModelNode(std::map<int, unsigned int>& vboList, tinygltf::Model& model, tinygltf::Node& node);
		void LoadMesh(std::map<int, unsigned int>& vboList, tinygltf::Model& model, tinygltf::Mesh& mesh);

		void RenderModelNode(tinygltf::Model& model, tinygltf::Node& node, Shader& shader);
		void RenderModelMesh(tinygltf::Model& model, tinygltf::Mesh& mesh, Shader& shader);

		//unsigned int m_VBO = 0;
		//unsigned int m_EBO = 0;
		//unsigned int m_VAO = 0;
		
		std::pair<GLuint, std::map<int, unsigned int>> vaoAndEbos;
		std::vector<size_t> m_Textures;

		bool didThing = false;
	};
}
