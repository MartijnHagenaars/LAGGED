#include "Platform/Resources/Mesh.h"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <TinyGLTF/tiny_gltf.h>

#include "Core/Engine.h"
#include "Core/Resources/ResourceManager.h"

#include "Platform/Resources/Texture.h"

#include "Utility/Logger.h"
#include "Utility/Hash.h"

namespace LAG
{
	struct VertexData
	{
		glm::vec3 vertices = glm::vec3(0.f);
		glm::vec3 normals = glm::vec3(0.f);;
		glm::vec2 textureCoords = glm::vec2(0.f);
	};

	namespace ModelAttributes
	{
		constexpr const char* POSITION = "POSITION";
		constexpr const char* NORMAL = "NORMAL";
		constexpr const char* TEXTURE_DIFFUSE = "TEXCOORD_0";
	}

	std::vector<VertexData> GetVertexData(const tinygltf::Model& model, const tinygltf::Primitive& primitive);
	std::vector<unsigned int> GetIndexData(const tinygltf::Model& model, const tinygltf::Primitive& primitive);

	//Gets the attribute data from a primitive accessor and stores it in a vector.
	std::vector<unsigned char> GetAttributeData(const tinygltf::Model& model, const tinygltf::Accessor& accessor);
	void AddAttributeDataToVertexBuffer(unsigned char* vertexData, const std::vector<unsigned char>& attributeData, int vertexDataSize, int vertexDataCount, int attributeSize, int attributeOffset);

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
					BufferLayoutElement(BufferVariableType::Float3, false),	//Position
					BufferLayoutElement(BufferVariableType::Float3, false),	//Normal
					BufferLayoutElement(BufferVariableType::Float2, false)	//Texture coordinates
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
		m_MeshBuffer.Shutdown();
	}

	void Mesh::Render()
	{
		for (size_t i = 0; i < m_Textures.size(); i++)
			GetResourceManager()->GetResource<Texture>(m_Textures.at(i))->Bind(i);

		m_MeshBuffer.Render();
	}

	std::vector<VertexData> GetVertexData(const tinygltf::Model& model, const tinygltf::Primitive& primitive)
	{
		std::vector<VertexData> vertData;
		//Load vertex data
		{
			//Check if the POSITION attribute exists.
			const auto& posAttribute = primitive.attributes.find(ModelAttributes::POSITION);
			if (posAttribute == primitive.attributes.end())
				CRITICAL("Primitive is missing {} attribute.", ModelAttributes::POSITION);

			const tinygltf::Accessor& accessor = model.accessors[posAttribute->second];

			//Resize the vertex data vector here so that attribute data can be stored.
			vertData.resize(accessor.count, VertexData());
			const auto& accessorData = GetAttributeData(model, accessor);
			AddAttributeDataToVertexBuffer(reinterpret_cast<unsigned char*>(&vertData.data()[0]), accessorData, sizeof(VertexData), vertData.size(), sizeof(glm::vec3), 0);
		}

		//Load normals
		{
			const auto& posAttribute = primitive.attributes.find(ModelAttributes::NORMAL);
			if (posAttribute == primitive.attributes.end())
				WARNING("Primitive is missing {} attribute.", ModelAttributes::NORMAL);

			const tinygltf::Accessor& accessor = model.accessors[posAttribute->second];
			const auto& accessorData = GetAttributeData(model, accessor);
			AddAttributeDataToVertexBuffer(reinterpret_cast<unsigned char*>(&vertData.data()[0]), accessorData, sizeof(VertexData), vertData.size(), sizeof(glm::vec3), sizeof(glm::vec3));
		}

		//Load texture coordinates
		{
			const auto& posAttribute = primitive.attributes.find(ModelAttributes::TEXTURE_DIFFUSE);
			if (posAttribute == primitive.attributes.end())
				WARNING("Primitive is missing {} attribute.", ModelAttributes::TEXTURE_DIFFUSE);

			const tinygltf::Accessor& accessor = model.accessors[posAttribute->second];
			if (accessor.componentType != TINYGLTF_COMPONENT_TYPE_FLOAT)
			{
				ERROR("Incorrect component type detected while loading mesh.");
				return vertData;
			}

			const auto& accessorData = GetAttributeData(model, accessor);
			AddAttributeDataToVertexBuffer(reinterpret_cast<unsigned char*>(&vertData.data()[0]), accessorData, sizeof(VertexData), vertData.size(), sizeof(glm::vec2), sizeof(glm::vec3) * 2);
		}

		return vertData;
	}

	std::vector<unsigned int> GetIndexData(const tinygltf::Model& model, const tinygltf::Primitive& primitive)
	{
		std::vector<unsigned int> indicesVec;

		const auto& indexAccessor = model.accessors[primitive.indices];
		const auto& indexBufferView = model.bufferViews[indexAccessor.bufferView];
		const auto& indexBuffer = model.buffers[indexBufferView.buffer];

		// Ensure the index type is correct
		//TODO: Instead of using for-loops, use a memcpy
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

	std::vector<unsigned char> GetAttributeData(const tinygltf::Model& model, const tinygltf::Accessor& accessor)
	{
		int accessorSize = tinygltf::GetComponentSizeInBytes(accessor.componentType) * tinygltf::GetNumComponentsInType(accessor.type);
		int bufferSize = accessorSize * accessor.count;

		std::vector<unsigned char> dataBuffer;
		dataBuffer.resize(bufferSize, 0);

		if (accessor.bufferView < 0)
			CRITICAL("Cannot load accessor data for {}: accessor doesn't have a buffer view.", !accessor.name.empty() ? accessor.name : "unnamed accessor");

		const auto& bufferView = model.bufferViews[accessor.bufferView];
		const auto& buffer = model.buffers[bufferView.buffer];

		const unsigned char* bufferSrcPtr = buffer.data.data() + bufferView.byteOffset + accessor.byteOffset;
		std::memcpy(&dataBuffer.data()[0], bufferSrcPtr, bufferSize);

		return dataBuffer;
	}

	void AddAttributeDataToVertexBuffer(unsigned char* vertexData, const std::vector<unsigned char>& attributeData,
		int vertexDataSize, int vertexDataCount, int attributeSize, int attributeOffset)
	{
		//Get the attribute source data and copy it to the destination
		const unsigned char* attributeSrcPtr = &attributeData.data()[0];
		unsigned char* attributeDestPtr = vertexData + attributeOffset;

		for (int i = 0; i < vertexDataCount; i++)
		{
			std::memcpy(attributeDestPtr, attributeSrcPtr, attributeSize);
			attributeDestPtr += vertexDataSize;
			attributeSrcPtr += attributeSize;
		}
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
			StringHash texHandle = StringHash(image.name);
			GetResourceManager()->AddResource<Texture>(texHandle, &image.image.data()[0], image.image.size(), image.width, image.height, LAG::TextureFormat::FORMAT_RGBA);
			textures.emplace_back(texHandle.GetValue());
		}

		return textures;
	}
}