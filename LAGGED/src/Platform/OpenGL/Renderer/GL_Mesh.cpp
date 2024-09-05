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

	std::vector<VertexData> GetVertexData(tinygltf::Model& modelData, tinygltf::Primitive& primitive);
	std::vector<unsigned int> GetIndexData(tinygltf::Model& modelData, tinygltf::Primitive& primitive);
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

		//TODO: We're assuming that only one primitive exists on the mesh. This is bad!
		if (meshData.primitives.size() != 1)
			CRITICAL("Primitive count is not equal to 1: {}", meshData.primitives.size());

		for (const auto& it : meshData.primitives)
		{
			std::vector<VertexData> vertexData = GetVertexData(modelData, meshData.primitives[0]);
			std::vector<unsigned int> indexData = GetIndexData(modelData, meshData.primitives[0]);
			m_Textures = LoadTexture(modelData, meshData.primitives[0]);
			m_TotalIndices = static_cast<unsigned int>(indexData.size());

			VertexBuffer vertexBuffer;
			vertexBuffer.SetLayout(
				std::vector<BufferLayoutElement>{
				BufferLayoutElement("a_Position", BufferVariableType::Float3, false),
					BufferLayoutElement("a_Normal", BufferVariableType::Float3, false),
					BufferLayoutElement("a_TextureCoords", BufferVariableType::Float2, false)
			});
			vertexBuffer.SetVertexData(vertexData.data(), vertexData.size() * sizeof(VertexData));

			IndexBuffer indexBuffer;
			indexBuffer.SetIndexData(indexData);
			m_MeshBuffer.Initialize(vertexBuffer, indexBuffer);
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

		m_MeshBuffer.Render();
	}

	std::vector<VertexData> GetVertexData(tinygltf::Model& modelData, tinygltf::Primitive& primitive)
	{
		std::vector<VertexData> vertData;

		//Load vertex data
		{

			const auto& primitiveAttributes = primitive.attributes["POSITION"];
			const auto& accessors = modelData.accessors[primitiveAttributes];
			const auto& bufferViews = modelData.bufferViews[accessors.bufferView];
			const auto& buffers = modelData.buffers[bufferViews.buffer];

			vertData.resize(accessors.count);
			const float* positions = reinterpret_cast<const float*>(&buffers.data[bufferViews.byteOffset + accessors.byteOffset]);
			for (size_t i = 0; i < vertData.capacity(); i++)
			{
				vertData[i].vertices = glm::vec3(positions[i * 3 + 0], positions[i * 3 + 1], positions[i * 3 + 2]);
			}
		}

		//Load normals
		{
			const auto& primitiveAttributes = primitive.attributes["NORMAL"];
			const auto& accessors = modelData.accessors[primitiveAttributes];
			const auto& bufferViews = modelData.bufferViews[accessors.bufferView];
			const auto& buffers = modelData.buffers[bufferViews.buffer];
			const float* normals = reinterpret_cast<const float*>(&buffers.data[bufferViews.byteOffset + accessors.byteOffset]);
			for (size_t i = 0; i < vertData.capacity(); i++)
			{
				vertData[i].normals = glm::vec3(normals[i * 3 + 0], normals[i * 3 + 1], normals[i * 3 + 2]);
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
			for (size_t i = 0; i < vertData.capacity(); i++)
			{
				vertData[i].textureCoords = glm::vec2(positions[i * 2 + 0], positions[i * 2 + 1]);
			}
		}

		return vertData;
	}

	std::vector<unsigned int> GetIndexData(tinygltf::Model& model, tinygltf::Primitive& primitive)
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

			//TODO: This is potentially dangerous! Using image.name as the texture handle could be dangerous. 
			// Should be prefixed with the model name
			HashedString texHandle = HashedString(image.name);
			GetResourceManager()->AddResource<Texture>(texHandle, &image.image.data()[0], image.image.size(), image.width, image.height, LAG::TextureFormat::FORMAT_RGBA);
			//TODO: This is annoying! 
			// Having to write "HashedString" when trying to look something up is annoying. Is preferably automatic.
			textures.emplace_back(texHandle.GetValue());
		}

		return textures;
	}
}