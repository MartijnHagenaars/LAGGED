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
		glm::vec3 vertices = glm::vec3(0.f);
		glm::vec3 normals = glm::vec3(0.f);;
		glm::vec2 textureCoords = glm::vec2(0.f);
	};

	namespace ModelAttributes
	{
		constexpr const char* POSITION = "POSITION";
		constexpr const char* NORMAL = "NORMAL";
		constexpr const char* TEXTURE_COORD = "TEXCOORD_0";
	}

	std::vector<VertexData> GetVertexData(const tinygltf::Model& model, const tinygltf::Primitive& primitive);
	std::vector<unsigned int> GetIndexData(const tinygltf::Model& model, const tinygltf::Primitive& primitive);

	//Gets the attribute data from a primitive accessor and stores it in a vector.
	std::vector<unsigned char> GetAttributeData(const tinygltf::Model& model, const tinygltf::Accessor& accessor);
	void AddAttributeDataToVertexBuffer(std::vector<VertexData>& vertexData, const std::vector<unsigned char>& attributeData, int attributeSize, int attributeOffset);

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
			AddAttributeDataToVertexBuffer(vertData, accessorData, sizeof(glm::vec3), 0);
		}

		//Load normals
		{
			const int primitiveAttributes = primitive.attributes.find(ModelAttributes::NORMAL)->second;
			const auto& accessors = model.accessors[primitiveAttributes];
			const auto& bufferViews = model.bufferViews[accessors.bufferView];
			const auto& buffers = model.buffers[bufferViews.buffer];
			const float* normals = reinterpret_cast<const float*>(&buffers.data[bufferViews.byteOffset + accessors.byteOffset]);
			for (size_t i = 0; i < vertData.capacity(); i++)
			{
				vertData[i].normals = glm::vec3(normals[i * 3 + 0], normals[i * 3 + 1], normals[i * 3 + 2]);
			}
		}

		//Load texture coordinates
		{
			const int primitiveAttributes = primitive.attributes.find(ModelAttributes::TEXTURE_COORD)->second;
			const auto& accessors = model.accessors[primitiveAttributes];
			const auto& bufferViews = model.bufferViews[accessors.bufferView];
			const auto& buffers = model.buffers[bufferViews.buffer];

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

	std::vector<unsigned int> GetIndexData(const tinygltf::Model& model, const tinygltf::Primitive& primitive)
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

	void AddAttributeDataToVertexBuffer(std::vector<VertexData>& vertexData, const std::vector<unsigned char>& attributeData,
		int attributeSize, int attributeOffset)
	{
		const int count = vertexData.size();
		const int stride = sizeof(VertexData);

		//Get the attribute source data and copy it to the destination
		const unsigned char* attributeSrcPtr = &attributeData.data()[0];
		VertexData* attributeDestPtr = &vertexData.data()[0] + attributeOffset;

		for (int i = 0; i < count; i++)
		{
			std::memcpy(attributeDestPtr, attributeSrcPtr, attributeSize);
			attributeDestPtr += 1; //TODO: Adding 1 since we're incrementing the VertexData* and not an unsigned char*. If we increment with the size of VertexData, we actually increment by 1024 (= sizeof(VertexData) * 32)
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
			HashedString texHandle = HashedString(image.name);
			GetResourceManager()->AddResource<Texture>(texHandle, &image.image.data()[0], image.image.size(), image.width, image.height, LAG::TextureFormat::FORMAT_RGBA);
			//TODO: This is annoying! 
			// Having to write "HashedString" when trying to look something up is annoying. Is preferably automatic.
			textures.emplace_back(texHandle.GetValue());
		}

		return textures;
	}
}