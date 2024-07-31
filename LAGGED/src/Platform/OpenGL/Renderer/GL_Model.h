#pragma once
#include "Core/Resources/Model.h"

#include "GL_Mesh.h"
#include "glm/mat4x4.hpp"

namespace LAG
{
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

		std::vector<Mesh> m_Meshes;

		struct Node
		{
			size_t m_MeshID;
			glm::mat4 m_LocalTransform;
		};
		std::vector<Node> m_Nodes;
		glm::mat4 m_MeshOffset;
	};
}
