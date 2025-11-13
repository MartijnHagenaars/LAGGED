#include "Platform/Resources/Model.h"

#include <GL/glew.h>
#include <glm/gtx/quaternion.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <TinyGLTF/tiny_gltf.h>

#include <filesystem>

#include "Core/Engine.h"
#include "Core/Resources/ResourceManager.h"

#include "ECS/Scene.h"
#include "ECS/Components/CameraComponent.h"
#include "ECS/Systems/CameraSystem.h"

#include "Platform/Resources/Shader.h"
#include "Platform/Resources/Texture.h"

#include "Utility/Logger.h"
#include "Utility/GL_ErrorChecks.h"

namespace LAG
{
	Model::Model(const StringHash64& path) :
		Resource(path)
	{
	}

	bool Model::Load()
	{
		std::unique_ptr<tinygltf::Model> modelData = std::make_unique<tinygltf::Model>();

		tinygltf::TinyGLTF modelLoader;

		std::string fileExtension;
		std::string filePath = GetPath().String();

		if (filePath.find_last_of(".") != std::string::npos)
			fileExtension = filePath.substr(filePath.find_last_of(".") + 1);
		else return false;

		bool loadSuccess = false;
		std::string errorMsg = "", warningMsg = "";

		if (fileExtension.compare("glb") == 0)
			loadSuccess = modelLoader.LoadBinaryFromFile(modelData.get(), &errorMsg, &warningMsg, filePath);
		else if (fileExtension.compare("gltf") == 0)
			loadSuccess = modelLoader.LoadASCIIFromFile(modelData.get(), &errorMsg, &warningMsg, filePath);
		else return false;

		//Check error messages
		if (!errorMsg.empty() || !loadSuccess)
			ERROR("Error while loading Model: {0}", errorMsg.empty() ? "No information available" : errorMsg);
		if (!warningMsg.empty())
			WARNING("Warning while loading Model: {0}", warningMsg);

		std::string modelDirPath = std::filesystem::path(filePath).parent_path().string();

		m_Meshes.resize(modelData->meshes.size());
		for (int i = 0; i < modelData->meshes.size(); i++)
		{
			m_Meshes[i].Load(GetPath().String(), *modelData, i);
		}

		m_Nodes.resize(modelData->nodes.size());
		for (int i = 0; i < modelData->nodes.size(); i++)
		{
			Node& node = m_Nodes[i];
			tinygltf::Node& gltfNode = modelData->nodes[i];

			int meshID = modelData->nodes[i].mesh;
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
				transformMat = transformMat * glm::toMat4(glm::quat((float)rotation[3], (float)rotation[0], (float)rotation[1], (float)rotation[2]));
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

	void LAG::Model::Render(EntityID objectEntityID, EntityID cameraEntityID, Shader& shader, std::vector<std::pair<TransformComponent*, LightComponent*>>& lights)
	{
		CameraComponent* camComp = GetScene()->GetComponent<CameraComponent>(cameraEntityID);

		shader.Bind();
		shader.SetMat4("a_ProjMat", camComp->projMat);
		shader.SetMat4("a_ViewMat", camComp->viewMat);

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
		{
			TransformComponent* objectTransformPtr = GetScene()->GetComponent<TransformComponent>(objectEntityID);
			UpdateTransformHierarchies(objectTransformPtr->GetTransformMatrix());
		}

		for (const auto& node : m_Nodes)
		{
			shader.SetMat4("a_ModelMat", node.m_GlobalTransform);
			m_Meshes[node.m_MeshID].Render();
		}
	}
}
