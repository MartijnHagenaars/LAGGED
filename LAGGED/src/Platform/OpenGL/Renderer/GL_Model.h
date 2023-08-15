#pragma once
#include "glm/glm.hpp"

namespace tinygltf
{
	class Model;
}

namespace LAG
{
	class Model
	{
	public:
		Model() = delete;
		explicit Model(const std::string& path);
		~Model();

		void Render();

		void SetPosition();
		void SetRotation();
		void SetScale();

	private:
		bool LoadModel(const std::string& path);

		void LoadTextures(const tinygltf::Model& modelData, const std::string& directoryPath);
		void LoadMeshes(const tinygltf::Model& modelData, const std::string& directoryPath);

		unsigned int m_VBO = 0;
		unsigned int m_EBO = 0;
		unsigned int m_VAO = 0;

		glm::vec3 m_Position = glm::vec3(0.f);
		glm::vec3 m_Rotation = glm::vec3(0.f);
		glm::vec3 m_Scale = glm::vec3(0.f);

		//tinygltf::Model m_Model = {};
		float m_PreTransformScale = 1.f; //????

	};
}
