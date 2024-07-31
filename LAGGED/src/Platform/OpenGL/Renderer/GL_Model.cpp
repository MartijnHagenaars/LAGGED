#include "GL_Model.h"
#include "GL_ErrorChecks.h"
#include "Platform/OpenGL/Renderer/GL_Shader.h" //TODO: BAD. Should use a general resource class instead of this platform-specific shit. Will also allow me to use it in the res manager

#include "Core/Engine.h"
#include "Utility/Logger.h"

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


//#ifndef STB_IMAGE_IMPLEMENTATION
//#define STB_IMAGE_IMPLEMENTATION
//#endif
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_NOEXCEPTION
#define JSON_NOEXCEPTION
#include "TinyGLTF/tiny_gltf.h"



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

		//Load data from file. GLB is used for binary files. 
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
			int meshID = m_Model->nodes[i].mesh;
			m_Nodes[i].m_MeshID = meshID;

			//Calculate the local transform

			glm::mat4 translationMatrix, rotationMatrix, scaleMatrix;
			
			//First, calculate the translation matrix
			if (!m_Model->nodes[meshID].translation.empty())
			{
				const auto& dblTranslate = m_Model->nodes[meshID].translation;
				translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(dblTranslate[0], dblTranslate[1], dblTranslate[2]));
			}
			else translationMatrix = glm::identity<glm::mat4>();

			//Next, the rotation matrix
			if (!m_Model->nodes[meshID].rotation.empty())
			{
				const auto& dblRotate = m_Model->nodes[meshID].rotation;
				rotationMatrix = glm::toMat4(glm::quat(dblRotate[0], dblRotate[0], dblRotate[0], dblRotate[0]));
			}
			else rotationMatrix = glm::identity<glm::mat4>();

			//Lastly, the scale matrix
			if (!m_Model->nodes[meshID].scale.empty())
			{
				const auto& dblScale = m_Model->nodes[meshID].scale;
				translationMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(dblScale[0], dblScale[1], dblScale[2]));
			}
			else scaleMatrix = glm::identity<glm::mat4>();

			//Combine all three matrices into one
			m_Nodes[i].m_LocalTransform = translationMatrix * rotationMatrix * scaleMatrix;
		}

		m_PreTransformScale = 1.f;
		return true;
	}

	bool Model::Unload()
	{
		for (auto& it : m_Meshes)
			it.Unload();
		return true;
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

		for (const auto& node : m_Nodes)
		{
			shader.SetMat4("a_ModelMat", transform.GetTransformMatrix() * node.m_LocalTransform);
			m_Meshes[node.m_MeshID].Render();
		}
	}
}
