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
	std::vector<size_t> LoadTexture(tinygltf::Model& modelData, tinygltf::Primitive& primitive);

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
			m_Textures = LoadTexture(modelData, meshData.primitives[0]);
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

	std::vector<unsigned int> LoadIndices(tinygltf::Model& model, tinygltf::Primitive& primitive)
	{
		std::vector<unsigned int> indicesVec;

		const auto& indexAccessor = model.accessors[primitive.indices];
		const auto& indexBufferView = model.bufferViews[indexAccessor.bufferView];
		const auto& indexBuffer = model.buffers[indexBufferView.buffer];

		// Ensure the index type is correct
		//TODO: Instead of using for-loops, check if we can use a memcpy
		if (indexAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT) 
		{
			const uint16_t* indices = reinterpret_cast<const uint16_t*>(&indexBuffer.data[indexBufferView.byteOffset + indexAccessor.byteOffset]);
			for (size_t i = 0; i < indexAccessor.count; i++) 
				indicesVec.push_back(indices[i]);
		}
		else if (indexAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT) 
		{
			const uint32_t* indices = reinterpret_cast<const uint32_t*>(&indexBuffer.data[indexBufferView.byteOffset + indexAccessor.byteOffset]);
			for (size_t i = 0; i < indexAccessor.count; i++) 
				indicesVec.push_back(indices[i]);
		}
		else CRITICAL("Mesh is using unsupported component type: {}", indexAccessor.componentType);

		return indicesVec;
	}

	std::vector<size_t> LoadTexture(tinygltf::Model& modelData, tinygltf::Primitive& primitive)
	{
		std::vector<size_t> textures;
		tinygltf::Material& material = modelData.materials[primitive.material];

		//Create a diffuse texture, if it exists.
		if (material.values.find("baseColorTexture") != material.values.end()) 
		{
			int diffuseTexIndex = material.values.at("baseColorTexture").TextureIndex();
			const tinygltf::Texture& texture = modelData.textures[diffuseTexIndex];
			const tinygltf::Image& image = modelData.images[texture.source];

			//TODO: THIS IS DUMB!
			// This is stupid! Instead of returning a boolean, it needs to return the pointer!
			// If the pointer is null, the resource could not be added. Otherwise, everything was successful.
			// Now I have to get the resource again which is really annoying.
			//TODO: This is potentially dangerous! Using image.name as the texture handle could be dangerous. 
			// Should be prefixed with the model name
			HashedString texHandle = HashedString(image.name);
			GetResourceManager()->AddResource<Texture>(texHandle);
			//TODO: This is annoying! 
			// Having to write "HashedString" when trying to look something up is annoying. Is preferably automatic. 
			//TODO: Doing this will cause issues, since texHandle is not a valid path! Need to reconsider how I want to do this in the resource manager!
			Texture* texPtr = GetResourceManager()->GetResource<Texture>(texHandle);
			texPtr->SetBuffer(reinterpret_cast<const float*>(&image.image), image.image.size(), image.width, image.height, LAG::TextureFormat::FORMAT_RGBA);
			textures.emplace_back(texHandle.GetValue());
		}

		return textures;
	}
}