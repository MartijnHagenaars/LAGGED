#pragma once
#include "Core/Resources/Model.h"

#include "GL_Mesh.h"
#include "glm/mat4x4.hpp"
#include "glm/gtx/quaternion.hpp"

namespace LAG
{
	class Entity;
	class Model : public ModelBase
	{
		friend class ResourceManager;
	public:
		Model() = delete;
		explicit Model(const HashedString& path);

		void Render(EntityID objectEntityID, EntityID cameraEntityID, Shader& shader, std::vector<std::pair<TransformComponent*, LightComponent*>>& lights) override;

	private:
		bool Load() override;
		bool Unload() override;

		struct Node;
		void UpdateTransformHierarchies(const glm::mat4& transformMat);
		void UpdateTransformHierarchy(Node& node, const glm::mat4& parentTransformMat);

		std::vector<Mesh> m_Meshes;

		struct Node
		{
			size_t m_MeshID;
			std::string m_DebugName;

			glm::mat4 m_LocalTransform = glm::identity<glm::mat4>();
			glm::mat4 m_GlobalTransform = glm::identity<glm::mat4>();

			Node* m_Parent;
			std::vector<Node*> m_Children;
		};
		std::vector<Node> m_Nodes;
		glm::mat4 m_MeshOffset;
	};
}
