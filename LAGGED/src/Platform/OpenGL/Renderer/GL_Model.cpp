#include "GL_Model.h"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_NOEXCEPTION
#define JSON_NOEXCEPTION
#include "TinyGLTF/tiny_gltf.h"

#include "Core/Engine.h"
#include "Utility/Logger.h"

#include "GL_ErrorChecks.h"
#include "Core/Resources/Shader.h"
#include "Core/Resources/ResourceManager.h"
#include "Core/Resources/Texture.h"

#include "ECS/Entity.h"
#include "ECS/Components/BasicComponents.h"
#include "ECS/Components/LightComponent.h"
#include "ECS/Components/CameraComponent.h"

#include "ECS/Systems/CameraSystem.h"

#include "GL/glew.h"

#include "glm/gtx/quaternion.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp" //make_vec3


namespace LAG
{
	Model::Model(const HashedString& path) :
		ModelBase(path)
	{
	}

	bool Model::Load()
	{
		if (m_Model == nullptr)
			m_Model = new tinygltf::Model();
		else
		{
			ERROR("Model \"{0}\" already exists.", GetPath().GetString());
			return false;
		}

		tinygltf::TinyGLTF modelLoader;

		std::string fileExtension;
		std::string filePath = GetPath().GetString();

		if (filePath.find_last_of(".") != std::string::npos)
			fileExtension = filePath.substr(filePath.find_last_of(".") + 1);
		else return false;

		bool loadSuccess = false;
		std::string errorMsg = "", warningMsg = "";

		if (fileExtension.compare("glb") == 0)
			loadSuccess = modelLoader.LoadBinaryFromFile(m_Model, &errorMsg, &warningMsg, filePath);
		else if (fileExtension.compare("gltf") == 0)
			loadSuccess = modelLoader.LoadASCIIFromFile(m_Model, &errorMsg, &warningMsg, filePath);
		else return false;

		//Check error messages
		if (!errorMsg.empty() || !loadSuccess)
			ERROR("Error while loading Model: {0}", errorMsg.empty() ? "No information available" : errorMsg);
		if (!warningMsg.empty())
			WARNING("Warning while loading Model: {0}", warningMsg);

		std::string modelDirPath = std::filesystem::path(filePath).parent_path().string();

		m_Meshes.resize(m_Model->meshes.size());
		for (int i = 0; i < m_Model->meshes.size(); i++)
		{
			m_Meshes[i].Load(GetPath().GetString(), *m_Model, i);
		}

		m_Nodes.resize(m_Model->nodes.size());
		for (int i = 0; i < m_Model->nodes.size(); i++)
		{
			Node& node = m_Nodes[i];
			tinygltf::Node& gltfNode = m_Model->nodes[i];

			int meshID = m_Model->nodes[i].mesh;
			node.m_MeshID = meshID;
			node.m_DebugName = gltfNode.name;

			node.m_Children.reserve(gltfNode.children.size());
			for (auto& childID : gltfNode.children)
				node.m_Children.emplace_back(&m_Nodes[childID]);

			//First, apply translation
			glm::mat4 transformMat = glm::identity<glm::mat4>();
			if (!gltfNode.translation.empty())
			{
				const auto& translation = gltfNode.translation;
				transformMat = glm::translate(transformMat, glm::vec3(translation[0], translation[1], translation[2]));
			}
			else transformMat = glm::translate(transformMat, glm::vec3(0.f));

			//Next, the rotation
			if (!gltfNode.rotation.empty())
			{
				const auto& rotation = gltfNode.rotation;
				transformMat = transformMat * glm::toMat4(glm::quat(rotation[3], rotation[0], rotation[1], rotation[2]));
			}
			else transformMat = transformMat * glm::toMat4(glm::identity<glm::quat>());

			//Lastly, the scale
			if (!gltfNode.scale.empty())
			{
				const auto& scale = gltfNode.scale;
				transformMat = glm::scale(transformMat, glm::vec3(scale[0], scale[1], scale[2]));
			}
			else transformMat = glm::scale(transformMat, glm::vec3(1.f));

			node.m_LocalTransform = transformMat;
		}

		for (auto& node : m_Nodes)
		{
			node.m_Parent = nullptr;
			for (auto& child : node.m_Children)
				child->m_Parent = &node;
		}

		return true;
	}

	bool Model::Unload()
	{
		for (auto& it : m_Meshes)
			it.Unload();
		return true;
	}

	void Model::UpdateTransformHierarchies(const glm::mat4& transformMat)
	{
		for (auto& node : m_Nodes)
		{
			if (node.m_Parent == nullptr)
				UpdateTransformHierarchy(node, transformMat);
		}
	}

	void Model::UpdateTransformHierarchy(Node& node, const glm::mat4& parentTransformMat)
	{
		node.m_GlobalTransform = parentTransformMat * node.m_LocalTransform;

		for (auto& child : node.m_Children)
			UpdateTransformHierarchy(*child, node.m_GlobalTransform);

	}

	void LAG::Model::Render(TransformComponent& transform, Entity* cameraEntity, Shader& shader, std::vector<std::pair<TransformComponent*, LightComponent*>>& lights)
	{
		shader.Bind();
		shader.SetMat4("a_ProjMat", CameraSystem::CalculateProjMat(cameraEntity));
		shader.SetMat4("a_ViewMat", CameraSystem::CalculateViewMat(cameraEntity));

		if (lights.size() > 0)
		{
			shader.SetBool("a_UseLight", true);
			for (int i = 0; i < TOTAL_POINT_LIGHTS; i++)
			{
				if (i < lights.size())
				{
					shader.SetVec3(std::string("a_PointLightData[" + std::to_string(i) + "].a_LightPosition"), lights[i].first->GetPosition());
					shader.SetVec3("a_PointLightData[" + std::to_string(i) + "].a_LightColor", lights[i].second->lightColor);
					shader.SetFloat("a_PointLightData[" + std::to_string(i) + "].a_LightIntensity", lights[i].second->lightIntensity);
					shader.SetFloat("a_PointLightData[" + std::to_string(i) + "].a_LightAttenuation", lights[i].second->lightAttenuation);
				}
			}
		}
		else shader.SetBool("a_UseLight", false);

		if (!m_Nodes.empty())
			UpdateTransformHierarchies(transform.GetTransformMatrix());

		for (const auto& node : m_Nodes)
		{
			shader.SetMat4("a_ModelMat", node.m_GlobalTransform);
			m_Meshes[node.m_MeshID].Render();
		}
	}
}
