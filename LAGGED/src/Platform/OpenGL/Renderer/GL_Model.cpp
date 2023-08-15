#include "Precomp.h"
#include "GL_Model.h"

#include <filesystem>

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
	Model::Model(const std::string& path)
	{
		LoadModel(path);
	}

	Model::~Model()
	{

	}

	bool Model::LoadModel(const std::string& path)
	{
		tinygltf::TinyGLTF modelLoader;
		tinygltf::Model model;

		std::string errorMsg = "No information available", warningMsg = "";

		bool loadSuccess = modelLoader.LoadASCIIFromFile(&model, &errorMsg, &warningMsg, path);
		if (!errorMsg.empty() || !loadSuccess)
			Utility::Logger::Error("Error while loading Model: {0}", errorMsg);
		if (!warningMsg.empty())
			Utility::Logger::Warning("Warning while loading Model: {0}", warningMsg);

		std::string modelDirPath = std::filesystem::path(path).parent_path().string();
		LoadTextures(model, modelDirPath);

		m_PreTransformScale = 1.f;
		return false;
	}

	void Model::LoadTextures(const tinygltf::Model& modelData, const std::string& directoryPath)
	{
		for (const auto& texture : modelData.textures)
		{
			std::string textureName = modelData.images[texture.source].uri; //Get texture resource identifier
			std::string texturePath = directoryPath + "/" + textureName;	//Get texture path for loading through resource manager
			Utility::Logger::Info("Loading texture at location {0}", texturePath);
		}
	}

	void Model::LoadMeshes(const tinygltf::Model& modelData, const std::string& directoryPath)
	{
	}

	void LAG::Model::Render()
	{

	}

	void LAG::Model::SetPosition()
	{
	}

	void LAG::Model::SetRotation()
	{
	}

	void LAG::Model::SetScale()
	{
	}
}
