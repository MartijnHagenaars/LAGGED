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
			LoadModelNode(vbos, modelData, modelData.nodes[scene.nodes[i]]);
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

		vaoAndEbos.first = vao;
		vaoAndEbos.second = vbos;
	}

	void Model::LoadModelNode(std::map<int, unsigned int>& vboList, tinygltf::Model& model, tinygltf::Node& node)
	{
		if ((node.mesh >= 0) && (node.mesh < model.meshes.size())) 
		{
			LoadMesh(vboList, model, model.meshes[node.mesh]);
		}

		for (size_t i = 0; i < node.children.size(); i++) 
		{
			assert((node.children[i] >= 0) && (node.children[i] < model.nodes.size()));
			LoadModelNode(vboList, model, model.nodes[node.children[i]]);
		}
	}

	void Model::LoadMesh(std::map<int, unsigned int>& vboList, tinygltf::Model& model, tinygltf::Mesh& mesh)
	{
		//Create and load the VBO
		for (size_t i = 0; i < model.bufferViews.size(); ++i)
		{
			const tinygltf::BufferView& bufferView = model.bufferViews[i];
			if (bufferView.target == 0)
				continue;

			const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];
			std::cout << "bufferview.target " << bufferView.target << std::endl;

			GLuint vbo; 
			LAG_GRAPHICS_EXCEPTION(glGenBuffers(1, &vbo));
			vboList[i] = vbo;

			LAG_GRAPHICS_EXCEPTION(glBindBuffer(bufferView.target, vbo));

			//TODO: Testing, remove.
			std::cout << "buffer.data.size = " << buffer.data.size()
				<< ", bufferview.byteOffset = " << bufferView.byteOffset
				<< std::endl;

			LAG_GRAPHICS_EXCEPTION(glBufferData(bufferView.target, bufferView.byteLength, &buffer.data.at(0) + bufferView.byteOffset, GL_STATIC_DRAW));
			auto afterInit = glGetError();
			printf("");
		}

		//Set the primitive attributes
		for (size_t i = 0; i < mesh.primitives.size(); ++i) 
		{
			tinygltf::Primitive primitive = mesh.primitives[i];
			tinygltf::Accessor indexAccessor = model.accessors[primitive.indices];

			for (auto& attrib : primitive.attributes) {
				tinygltf::Accessor accessor = model.accessors[attrib.second];
				int byteStride = accessor.ByteStride(model.bufferViews[accessor.bufferView]);
				
				//TODO: This sucks. Doesn't work with multiple meshes. REPLACE!

				LAG_GRAPHICS_EXCEPTION(glBindBuffer(GL_ARRAY_BUFFER, vboList[accessor.bufferView]));
				//glBindBuffer(GL_ARRAY_BUFFER, vbos[accessor.bufferView]);

				int size = 1;
				if (accessor.type != TINYGLTF_TYPE_SCALAR) {
					size = accessor.type;
				}

				int vaa = -1;
				if (attrib.first.compare("POSITION") == 0) vaa = 0;
				if (attrib.first.compare("NORMAL") == 0) vaa = 1;
				if (attrib.first.compare("TEXCOORD_0") == 0) vaa = 2;
				if (vaa > -1) {
					LAG_GRAPHICS_EXCEPTION(glEnableVertexAttribArray(vaa));
					LAG_GRAPHICS_EXCEPTION(glVertexAttribPointer(vaa, size, accessor.componentType,
						accessor.normalized ? GL_TRUE : GL_FALSE,
						byteStride, ((char*)NULL + (accessor.byteOffset))));
				}
				else
					std::cout << "vaa missing: " << attrib.first << std::endl;
			}
		}

		//Load textures
		for (const auto& texture : model.textures)
		{
			std::string textureName = model.images[texture.source].uri; //Get texture resource identifier

			std::string modelPath = GetPath().GetString();
			std::string texturePath = modelPath.erase(modelPath.find_last_of('/'), modelPath.length() - 1 - 1) + "/" + textureName; //Get texture path for loading through resource manager
			Utility::Logger::Info("Loading texture at location {0}", texturePath);
			ResourceManager::Get().AddResource<Texture>(Utility::String(texturePath.c_str()));
		}
	}

	void LAG::Model::Render(Shader& shader)
	{
		std::cout << sinf((float)glfwGetTime()) * 10.f << std::endl;
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

		LAG_GRAPHICS_EXCEPTION(glBindVertexArray(vaoAndEbos.first));

		tinygltf::Scene& scene = m_Model->scenes[m_Model->defaultScene];
		for (size_t i = 0; i < scene.nodes.size(); ++i) 
		{
			RenderModelNode(*m_Model, m_Model->nodes[scene.nodes[i]]);
		}

		auto afterRender = glGetError();
	}

	void LAG::Model::RenderModelNode(tinygltf::Model& model, tinygltf::Node& node)
	{
		if ((node.mesh >= 0) && (node.mesh < model.meshes.size())) 
		{
			RenderModelMesh(model, model.meshes[node.mesh]);
		}
		for (size_t i = 0; i < node.children.size(); i++) 
		{
			RenderModelNode(model, model.nodes[node.children[i]]);
		}
	}

	void LAG::Model::RenderModelMesh(tinygltf::Model& model, tinygltf::Mesh& mesh)
	{
		for (size_t i = 0; i < mesh.primitives.size(); ++i) 
		{
			tinygltf::Primitive primitive = mesh.primitives[i];
			tinygltf::Accessor indexAccessor = model.accessors[primitive.indices];

			//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos.at(indexAccessor.bufferView));
			LAG_GRAPHICS_EXCEPTION(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vaoAndEbos.second.at(indexAccessor.bufferView)));
			
			auto preRender = glGetError();
			LAG_GRAPHICS_EXCEPTION(glDrawElements(primitive.mode, indexAccessor.count,
				indexAccessor.componentType,
				((char*)NULL + (indexAccessor.byteOffset))));
			auto afterRender = glGetError(); //Error!
			if (afterRender != 0)
			{
				LAG_ASSERT("Rendering error!!");
			}
		}
	}
}
