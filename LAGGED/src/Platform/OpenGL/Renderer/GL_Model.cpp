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
	Model::Model(const Utility::String& path) :
		ModelBase(path)
	{
	}

	Model::~Model()
	{

	}

	bool Model::Load()
	{
		tinygltf::TinyGLTF modelLoader;
		tinygltf::Model model;

		std::string fileExtension;
		std::string filePath = GetPath().GetString();

		if (filePath.find_last_of(".") != std::string::npos)
			fileExtension = fileExtension.substr(fileExtension.find_last_of(".") + 1);
		else return false;

		bool loadSuccess = false;
		std::string errorMsg = "", warningMsg = "";

		//Load data from file. GLB is used for binary files. 
		if(fileExtension.compare("glb") == 0)
			loadSuccess = modelLoader.LoadBinaryFromFile(&model, &errorMsg, &warningMsg, filePath);
		else loadSuccess = modelLoader.LoadASCIIFromFile(&model, &errorMsg, &warningMsg, filePath);

		//Check error messages
		if (!errorMsg.empty() || !loadSuccess)
			Utility::Logger::Error("Error while loading Model: {0}", errorMsg.empty() ? "No information available" : errorMsg);
		if (!warningMsg.empty())
			Utility::Logger::Warning("Warning while loading Model: {0}", warningMsg);

		std::string modelDirPath = std::filesystem::path(filePath).parent_path().string();
		LoadTextures(model, modelDirPath);
		LoadMeshes(model, modelDirPath);

		m_PreTransformScale = 1.f;
		return false;
	}

	bool Model::Unload()
	{
		//TODO: Currently doesn't do anything
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
		for (const auto& mesh : modelData.meshes)
		{

		}
	}

	void LAG::Model::Render()
	{

	}
}
