#pragma once
#include "Core/Resources/Model.h"
#include "Platform/Base/Renderer/Buffer.h"

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

#include <map>
#include <utility>
#include <vector>

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

	class Entity;
	class Model : public ModelBase
	{
		friend class ResourceManager;
	public:
		Model() = delete;
		explicit Model(const HashedString& path);

		void Render(TransformComponent& transform, Entity* cameraEntity, Shader& shader, std::vector<std::pair<TransformComponent*, LightComponent*>>& lights) override;

	private:
		bool Load() override;
		bool Unload() override;

		void LoadModel(tinygltf::Model& modelData) override;
		
		ArrayBuffer m_Buffer;

		std::vector<size_t> m_Textures;

		unsigned int m_TotalIndices = 0;
	};
}
