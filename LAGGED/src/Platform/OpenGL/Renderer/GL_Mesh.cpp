#include "GL_Mesh.h"

#include "Core/Engine.h"
#include "Core/Resources/ResourceManager.h"
#include "Core/Resources/Texture.h"

#include "TinyGLTF/tiny_gltf.h"
#include "Utility/Logger.h"

#include "Utility/HashedString.h"

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

namespace LAG
{
	struct VertexData
	{
		glm::vec3 vertices;
		glm::vec3 normals;
		glm::vec2 textureCoords;
	};

	std::vector<VertexData> LoadVertices(tinygltf::Model& modelData, tinygltf::Primitive& primitive);
	std::vector<unsigned int> LoadIndices(tinygltf::Model& modelData, tinygltf::Primitive& primitive);
	std::vector<size_t> LoadTexture(tinygltf::Model& modelData, std::string modelPath);

	bool Mesh::Load(const std::string& path, tinygltf::Model& modelData, size_t meshIndex)
	{
		if (meshIndex < 0 || meshIndex >= modelData.meshes.size())
			CRITICAL("Invalid mesh index: {}", meshIndex);
		tinygltf::Mesh& meshData = modelData.meshes[meshIndex];

		//Check if there's primitive data
		if (meshData.primitives.size() <= 0)
		{
			ERROR("Failed to load mesh {}.", meshData.name);
			return false;
		}

		INFO("Primitive count: {}", meshData.primitives.size());
		for (const auto& it : meshData.primitives)
		{
			//Index 0 is wrong!
			std::vector<VertexData> vertexData = LoadVertices(modelData, meshData.primitives[0]);
			std::vector<unsigned int> indices = LoadIndices(modelData, meshData.primitives[0]);
			m_Textures = LoadTexture(modelData, path);
			m_TotalIndices = static_cast<unsigned int>(indices.size());

			VertexBuffer vertexBuffer;
			vertexBuffer.SetLayout(
				std::vector<BufferLayoutElement>{
				BufferLayoutElement("a_Position", BufferVariableType::Float3, false),
					BufferLayoutElement("a_Normal", BufferVariableType::Float3, false),
					BufferLayoutElement("a_TextureCoords", BufferVariableType::Float2, false)
			});
			vertexBuffer.SetVertexData(vertexData.data(), vertexData.size() * sizeof(VertexData));

			IndexBuffer indexBuffer;
			indexBuffer.SetIndexData(indices);
			m_Buffer.Initialize(vertexBuffer, indexBuffer);
		}




		return true;
	}

	void Mesh::Unload()
	{
		CRITICAL("FAILED TO UNLOAD MESH");
	}

	void Mesh::Render()
	{
		for (size_t i = 0; i < m_Textures.size(); i++)
			GetResourceManager()->GetResource<Texture>(m_Textures.at(i))->Bind(i);

		m_Buffer.Render();
	}

	std::vector<VertexData> LoadVertices(tinygltf::Model& modelData, tinygltf::Primitive& primitive)
	{
		tinygltf::Model mod;
		std::vector<VertexData> meshData;

		//Load vertex data
		{
			
			const auto& primitiveAttributes = primitive.attributes["POSITION"];
			const auto& accessors = modelData.accessors[primitiveAttributes];
			const auto& bufferViews = modelData.bufferViews[accessors.bufferView];
			const auto& buffers = modelData.buffers[bufferViews.buffer];

			meshData.resize(accessors.count);
			const float* positions = reinterpret_cast<const float*>(&buffers.data[bufferViews.byteOffset + accessors.byteOffset]);
			for (size_t i = 0; i < meshData.capacity(); i++)
			{
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
				ERROR("Incorrect component type detected while loading mesh.");
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
		INFO("Loading texture at location {0}", texturePath);

		textures.emplace_back(pathString.GetValue());
		GetResourceManager()->GetResource<Texture>(pathString)->Unbind(0);

		return textures;
	}
}