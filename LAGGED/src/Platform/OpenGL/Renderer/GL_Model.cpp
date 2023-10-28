#include "Precomp.h"
#include "GL_Model.h"

#include "Core/Engine.h"
#include "Platform/Base/Window/WindowManager.h"
#include "Core/Resources/ResourceManager.h"
#include "Core/Resources/Texture.h"
#include "Platform/OpenGL/Renderer/GL_Shader.h" //TODO: BAD. Should use a general resource class instead of this platform-specific shit. Will also allow me to use it in the res manager

#include "Platform/OpenGL/Renderer/Exceptions/GL_GraphicsExceptionMacros.h"

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
	Model::Model(const Utility::String& path) :
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
			Utility::Logger::Error("Model \"{0}\" already exists.", GetPath().GetString());
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
			Utility::Logger::Error("Error while loading Model: {0}", errorMsg.empty() ? "No information available" : errorMsg);
		if (!warningMsg.empty())
			Utility::Logger::Warning("Warning while loading Model: {0}", warningMsg);

		std::string modelDirPath = std::filesystem::path(filePath).parent_path().string();
		LoadModel(*m_Model, modelDirPath);

		m_PreTransformScale = 1.f;
		return true;
	}

	bool Model::Unload()
	{
		//TODO: Currently doesn't do anything
		return false;
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
				Utility::Logger::Error("Incorrect component type detected while loading mesh.");
			}

			const float* positions = reinterpret_cast<const float*>(&buffers.data[bufferViews.byteOffset + accessors.byteOffset]);
			for (size_t i = 0; i < meshData.capacity(); i++)
			{
				meshData[i].textureCoords = glm::vec2(positions[i * 2 + 0], positions[i * 2 + 1]);
			}
		}

		return meshData;
	}

	std::vector<unsigned short> LoadIndices(tinygltf::Model& modelData, tinygltf::Primitive& primitive)
	{
		std::vector<unsigned short> indices;

		const auto& accessors = modelData.accessors[primitive.indices];
		const auto& bufferViews = modelData.bufferViews[accessors.bufferView];
		const auto& buffers = modelData.buffers[bufferViews.buffer];
		auto whatever = accessors.componentType;
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

	std::vector<size_t> LoadTexture(tinygltf::Model& modelData, tinygltf::Primitive& primitive, std::string modelPath)
	{
		std::vector<size_t> textures;

		tinygltf::Texture& texture = modelData.textures.at(0);
		std::string textureName = modelData.images[texture.source].uri; //Get texture resource identifier

		std::string texturePath = modelPath.erase(modelPath.find_last_of('/'), modelPath.length() - 1 - 1) + "/" + textureName; //Get texture path for loading through resource manager

		Utility::String pathString = Utility::String(texturePath.c_str());

		GetResourceManager()->AddResource<Texture>(pathString);
		Utility::Logger::Info("Loading texture at location {0}", texturePath);

		textures.emplace_back(pathString.GetValue());
		GetResourceManager()->GetResource<Texture>(pathString)->Unbind(0);

		return textures;
	}

	void Model::LoadModel(tinygltf::Model& modelData, std::string& directoryPath)
	{
		//Create buffer objects and such
		LAG_GRAPHICS_EXCEPTION(glGenVertexArrays(1, &m_VAO));
		LAG_GRAPHICS_EXCEPTION(glGenBuffers(1, &m_VBO));
		LAG_GRAPHICS_EXCEPTION(glGenBuffers(1, &m_EBO));

		//For now, I'm only looking at the first mesh. In the future, this should loop and create mesh objects. TODO!
		auto& primitive = modelData.meshes[0].primitives[0];
		std::vector<MeshData> meshData = LoadVertices(modelData, primitive);
		std::vector<unsigned short> indices = LoadIndices(modelData, primitive);
		m_Textures = LoadTexture(modelData, primitive, GetPath().GetString());
		m_TotalIndices = indices.size();

		LAG_GRAPHICS_EXCEPTION(glBindVertexArray(m_VAO));
		
		LAG_GRAPHICS_EXCEPTION(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
		LAG_GRAPHICS_EXCEPTION(glBufferData(GL_ARRAY_BUFFER, sizeof(MeshData) * meshData.size(), &meshData.data()[0], GL_STATIC_DRAW));
		
		LAG_GRAPHICS_EXCEPTION(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO));
		LAG_GRAPHICS_EXCEPTION(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * indices.size(), &indices.data()[0], GL_STATIC_DRAW));
		
		LAG_GRAPHICS_EXCEPTION(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0));
		LAG_GRAPHICS_EXCEPTION(glEnableVertexAttribArray(0));

		LAG_GRAPHICS_EXCEPTION(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))));
		LAG_GRAPHICS_EXCEPTION(glEnableVertexAttribArray(1));

		LAG_GRAPHICS_EXCEPTION(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))));
		LAG_GRAPHICS_EXCEPTION(glEnableVertexAttribArray(2));
		
		LAG_GRAPHICS_EXCEPTION(glBindVertexArray(0));
	}

	void LAG::Model::Render(TransformComponent& transform, Shader& shader)
	{
		glm::mat4 modelMat = glm::mat4(1.f);
		modelMat = glm::translate(modelMat, transform.position);
		modelMat = glm::rotate(modelMat, transform.rotation.x, glm::vec3(0.5f, 0.5f, 0.f)); //Shit rotation calculation. FIX!!!
		modelMat = glm::scale(modelMat, transform.scale);

		glm::mat4 viewMat = glm::mat4(1.f);
		viewMat = glm::translate(viewMat, glm::vec3(0.f, 0.f, 1.f));
		glm::mat4 projMat = glm::mat4(1.f);

		//Will be moved when camera gets added.
		projMat = glm::perspective(glm::radians(45.f), static_cast<float>(GetWindowManager()->GetFocussedWindow()->GetWidth()) / GetWindowManager()->GetFocussedWindow()->GetHeight(), 0.1f, 100.f);

		shader.Bind();
		shader.SetMat4("a_ModelMat", modelMat);
		shader.SetMat4("a_ViewMat", viewMat);
		shader.SetMat4("a_ProjMat", projMat);

		shader.SetVec3("a_LightPosition", glm::vec3(5.f, -15.f, 0.f));
		shader.SetVec3("a_LightColor", glm::vec3(1.f, 0.f, 0.f));
		shader.SetFloat("a_LightIntensity", 1.f);
		
		//Bind textures
		for (size_t i = 0; i < m_Textures.size(); i++)
			GetResourceManager()->GetResource<Texture>(m_Textures.at(i))->Bind(i);

		LAG_GRAPHICS_EXCEPTION(glBindVertexArray(m_VAO));
		LAG_GRAPHICS_EXCEPTION(glDrawElements(GL_TRIANGLES, m_TotalIndices, GL_UNSIGNED_SHORT, 0));
	}
}
