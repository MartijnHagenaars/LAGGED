#include "Precomp.h"
#include "GL_Model.h"

#include "Core/Resources/ResourceManager.h"
#include "Platform/OpenGL/Renderer/GL_Shader.h" //TODO: BAD. Should use a general resource class instead of this platform-specific shit. Will also allow me to use it in the res manager
#include "Platform/OpenGL/Renderer/Exceptions/GL_GraphicsExceptionMacros.h"
#include "Platform/OpenGL/Renderer/GL_Texture.h" //TODO: BAD

#include <filesystem>
#include "GL/glew.h"
#include "GLFW/glfw3.h" //REMOVE: USED FOR TESTING
#include "Platform/Base/Window/WindowManager.h" //TODO: SAME FOR THIS ONE!

//#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

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

	void Model::LoadTextures(tinygltf::Model& modelData, std::string& directoryPath)
	{
		//for (const auto& texture : modelData.textures)
		//{
		//	std::string textureName = modelData.images[texture.source].uri; //Get texture resource identifier
		//	std::string texturePath = directoryPath + "/" + textureName;	//Get texture path for loading through resource manager
		//	Utility::Logger::Info("Loading texture at location {0}", texturePath);
		//}
	}

	void Model::LoadModel(tinygltf::Model& modelData, std::string& directoryPath)
	{
		LAG_GRAPHICS_EXCEPTION(glGenVertexArrays(1, &m_VAO));
		LAG_GRAPHICS_EXCEPTION(glBindVertexArray(m_VAO));

		//For now, I'm only looking at the first mesh. In the future, this should loop and create mesh objects. 
		auto& primitive = modelData.meshes[0].primitives[0];

		//Load vertex data
		const auto& primitiveAttributes = primitive.attributes["POSITION"];
		const auto& accessors = modelData.accessors[primitiveAttributes];
		const auto& bufferViews = modelData.bufferViews[accessors.bufferView];

		std::vector<glm::vec3> vertices;
		vertices.reserve(accessors.count);
		const float* positions = reinterpret_cast<const float*>(&modelData.buffers[bufferViews.buffer].data[bufferViews.byteOffset + accessors.byteOffset]);
		for (size_t i = 0; i < vertices.capacity(); i++)
		{
			vertices.emplace_back(glm::vec3(positions[i * 3 + 0], positions[i * 3 + 1], positions[i * 3 + 2]));
			std::cout << "(" << positions[i * 3 + 0] << ", " << positions[i * 3 + 1] << ", " << positions[i * 3 + 2] << ")" << "\n";
		}

		LAG_GRAPHICS_EXCEPTION(glBindVertexArray(0));
	}

	void LAG::Model::Render(Shader& shader)
	{
		glm::mat4 modelMat = glm::mat4(1.f);
		modelMat = glm::translate(modelMat, glm::vec3(sinf((float)glfwGetTime()) * 2.f, 0.f, -10.f));
		modelMat = glm::rotate(modelMat, (float)glfwGetTime(), glm::vec3(0.5f, 0.5f, 0.f));
		modelMat = glm::scale(modelMat, m_Scale);

		glm::mat4 viewMat = glm::mat4(1.f);
		viewMat = glm::translate(viewMat, glm::vec3(0.f, 0.f, 1.f));

		glm::mat4 projMat = glm::mat4(1.f);
		projMat = glm::perspective(glm::radians(45.f), static_cast<float>(WindowManager::Get().GetFocussedWindow()->GetWidth() / WindowManager::Get().GetFocussedWindow()->GetHeight()), 0.1f, 100.f);
		shader.Bind();
		shader.SetMat4("modelMat", modelMat);
		shader.SetMat4("viewMat", viewMat);
		shader.SetMat4("projMat", projMat);
	}
}
