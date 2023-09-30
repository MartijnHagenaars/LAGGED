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
			fileExtension = fileExtension.substr(fileExtension.find_last_of(".") + 1);
		else return false;

		bool loadSuccess = false;
		std::string errorMsg = "", warningMsg = "";

		//Load data from file. GLB is used for binary files. 
		if(fileExtension.compare("glb") == 0)
			loadSuccess = modelLoader.LoadBinaryFromFile(m_Model, &errorMsg, &warningMsg, filePath);
		else loadSuccess = modelLoader.LoadASCIIFromFile(m_Model, &errorMsg, &warningMsg, filePath);

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
		GLuint vao; 
		std::map<int, unsigned int> vbos;

		
		LAG_GRAPHICS_EXCEPTION(glGenVertexArrays(1, &vao)); //TODO: Research what the difference is between this func and "glCreateVertexArrays"?
		LAG_GRAPHICS_EXCEPTION(glBindVertexArray(vao));

		//Note: Might want to loop over this instead. TODO: Research!
		const tinygltf::Scene& scene = modelData.scenes[modelData.defaultScene];
		for (size_t i = 0; i < scene.nodes.size(); i++)
		{
			
		}

		LAG_GRAPHICS_EXCEPTION(glBindVertexArray(0));

		for (auto it = vbos.cbegin(); it != vbos.cend();) {
			tinygltf::BufferView bufferView = modelData.bufferViews[it->first];
			if (bufferView.target != GL_ELEMENT_ARRAY_BUFFER) 
			{
				LAG_GRAPHICS_EXCEPTION(glDeleteBuffers(1, &vbos[it->first]));
				vbos.erase(it++);
			}
			else 
			{
				++it;
			}
		}
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

		auto preRender = glGetError();

		//LAG_GRAPHICS_EXCEPTION(glBindVertexArray(...));

		tinygltf::Scene& scene = m_Model->scenes[m_Model->defaultScene];
		for (size_t i = 0; i < scene.nodes.size(); ++i) 
		{
			
		}

		auto afterRender = glGetError();
	}
}
