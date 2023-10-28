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
	struct MeshData
	{
		glm::vec3 vertices;
		glm::vec3 normals;
		glm::vec2 textureCoords;
	};

	class Model : public ModelBase
	{
		friend class ResourceManager;
	public:
		Model() = delete;
		explicit Model(const Utility::String& path);
		~Model() override;

		void Render(TransformComponent& transform, Shader& shader, std::vector<std::pair<TransformComponent*, LightComponent*>>& lights) override;

	private:
		bool Load() override;
		bool Unload() override;

		void LoadTextures(tinygltf::Model& modelData, std::string& directoryPath) override {}; //CURRENTLY NOT BEING USED
		void LoadModel(tinygltf::Model& modelData, std::string& directoryPath) override;
		
		std::vector<size_t> m_Textures;

		unsigned int m_VBO = 0;
		unsigned int m_EBO = 0;
		unsigned int m_VAO = 0;

		unsigned int m_TotalIndices = 0;
	};
}
