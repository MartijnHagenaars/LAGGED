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
#include "glm/gtc/matrix_transform.hpp"


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

	Model::~Model()
	{

	}

	bool Model::Load()
	{
		if (m_Model == nullptr)
			m_Model = new tinygltf::Model();
		else
		{
			Logger::Error("Model \"{0}\" already exists.", GetPath().GetString());
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
			Logger::Error("Error while loading Model: {0}", errorMsg.empty() ? "No information available" : errorMsg);
		if (!warningMsg.empty())
			Logger::Warning("Warning while loading Model: {0}", warningMsg);

		std::string modelDirPath = std::filesystem::path(filePath).parent_path().string();
		LoadModel(*m_Model);

		m_PreTransformScale = 1.f;
		return true;
	}

	bool Model::Unload()
	{
		//TODO: Unload the model
		// 
		//glDeleteBuffers(1, &m_VBO);
		//glDeleteBuffers(1, &m_EBO);
		//glDeleteBuffers(1, &m_VAO);

		return true;
	}

	std::vector<MeshData> LoadVertices(tinygltf::Model& modelData, tinygltf::Primitive& primitive)
	{
		std::vector<MeshData> meshData;

		//Load vertex data
		{
			const auto& primitiveAttributes = primitive.attributes["POSITION"];
			const auto& accessors = modelData.accessors[primitiveAttributes];
			const auto& bufferViews = modelData.bufferViews[accessors.bufferView];
			const auto& buffers = modelData.buffers[bufferViews.buffer];

			meshData.reserve(accessors.count);
			const float* positions = reinterpret_cast<const float*>(&buffers.data[bufferViews.byteOffset + accessors.byteOffset]);
			for (size_t i = 0; i < meshData.capacity(); i++)
			{
				meshData.emplace_back(MeshData({}));
				meshData[i].vertices = glm::vec3(positions[i * 3 + 0], positions[i * 3 + 1], positions[i * 3 + 2]);
			}
		}

		//Load normals
		{
			const auto& primitiveAttributes = primitive.attributes["NORMAL"];
			const auto& accessors = modelData.accessors[primitiveAttributes];
			const auto& bufferViews = modelData.bufferViews[accessors.bufferView];
			const auto& buffers = modelData.buffers[bufferViews.buffer];
			const float* normals = reinterpret_cast<const float*>(&buffers.data[bufferViews.byteOffset + accessors.byteOffset]);
			for (size_t i = 0; i < meshData.capacity(); i++)
			{
				meshData[i].normals = glm::vec3(normals[i * 3 + 0], normals[i * 3 + 1], normals[i * 3 + 2]);
			}
		}

		//Load texture coordinates
		{
			const auto& primitiveAttributes = primitive.attributes["TEXCOORD_0"];
			const auto& accessors = modelData.accessors[primitiveAttributes];
			const auto& bufferViews = modelData.bufferViews[accessors.bufferView];
			const auto& buffers = modelData.buffers[bufferViews.buffer];

			if (accessors.componentType != TINYGLTF_COMPONENT_TYPE_FLOAT)
			{
				Logger::Error("Incorrect component type detected while loading mesh.");
			}

			const float* positions = reinterpret_cast<const float*>(&buffers.data[bufferViews.byteOffset + accessors.byteOffset]);
			for (size_t i = 0; i < meshData.capacity(); i++)
			{
				meshData[i].textureCoords = glm::vec2(positions[i * 2 + 0], positions[i * 2 + 1]);
			}
		}

		return meshData;
	}

	std::vector<unsigned int> LoadIndices(tinygltf::Model& modelData, tinygltf::Primitive& primitive)
	{
		std::vector<unsigned int> indices;

		const auto& accessors = modelData.accessors[primitive.indices];
		const auto& bufferViews = modelData.bufferViews[accessors.bufferView];
		const auto& buffers = modelData.buffers[bufferViews.buffer];
		size_t accessorSize = tinygltf::GetNumComponentsInType(accessors.type) * tinygltf::GetComponentSizeInBytes(accessors.componentType);
		indices.reserve((bufferViews.byteLength / accessorSize));
		{
			auto* indexArray = &buffers.data.data()[bufferViews.byteOffset];
			const unsigned short* indexArrayShort = reinterpret_cast<const unsigned short*>(indexArray);

			for (size_t i = 0; i < indices.capacity(); i++)
			{
				indices.emplace_back(indexArrayShort[i]);
			}
		}

		return indices;
	}

	std::vector<size_t> LoadTexture(tinygltf::Model& modelData, std::string modelPath)
	{
		std::vector<size_t> textures;

		tinygltf::Texture& texture = modelData.textures.at(0);
		std::string textureName = modelData.images[texture.source].uri; //Get texture resource identifier

		std::string texturePath = modelPath.erase(modelPath.find_last_of('/'), modelPath.length() - 1 - 1) + "/" + textureName; //Get texture path for loading through resource manager

		HashedString pathString = HashedString(texturePath.c_str());

		GetResourceManager()->AddResource<Texture>(pathString);
		Logger::Info("Loading texture at location {0}", texturePath);

		textures.emplace_back(pathString.GetValue());
		GetResourceManager()->GetResource<Texture>(pathString)->Unbind(0);

		return textures;
	}

	void Model::LoadModel(tinygltf::Model& modelData)
	{
		//For now, I'm only looking at the first mesh. In the future, this should loop and create mesh objects. TODO!
		auto& primitive = modelData.meshes[0].primitives[0];
		std::vector<MeshData> meshData = LoadVertices(modelData, primitive);
		std::vector<unsigned int> indices = LoadIndices(modelData, primitive);
		m_Textures = LoadTexture(modelData, GetPath().GetString());
		m_TotalIndices = static_cast<unsigned int>(indices.size());

		VertexBuffer vertexBuffer;
		vertexBuffer.SetLayout(
			std::vector<BufferLayoutElement>{
			BufferLayoutElement("a_Position", BufferVariableType::Float3, false), 
			BufferLayoutElement("a_Normal", BufferVariableType::Float3, false),
			BufferLayoutElement("a_TextureCoords", BufferVariableType::Float2, false) 
			});
		vertexBuffer.SetVertexData(meshData.data(), meshData.size() * sizeof(MeshData));

		IndexBuffer indexBuffer;
		indexBuffer.SetIndexData(indices);
		m_Buffer.Initialize(vertexBuffer, indexBuffer);
	}

	void LAG::Model::Render(TransformComponent& transform, Entity* cameraEntity, Shader& shader, std::vector<std::pair<TransformComponent*, LightComponent*>>& lights)
	{
		shader.Bind();
		shader.SetMat4("a_ModelMat", transform.GetTransformMatrix());
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

		
		//Bind textures
		for (size_t i = 0; i < m_Textures.size(); i++)
			GetResourceManager()->GetResource<Texture>(m_Textures.at(i))->Bind(i);

		m_Buffer.Render();

		//LAG_GRAPHICS_CHECK(glBindVertexArray(m_VAO));
		//LAG_GRAPHICS_CHECK(glDrawElements(GL_TRIANGLES, m_TotalIndices, GL_UNSIGNED_SHORT, 0));
	}
}
