#pragma once
#include "glm/glm.hpp"
#include "json/json.hpp"

namespace LAG
{
	using json = nlohmann::json;
	class Mesh
	{
	public:
		Mesh() = delete;
		explicit Mesh(const std::string& path);
		~Mesh();

		void Render();

		void SetPosition();
		void SetRotation();
		void SetScale();

	private:
		bool LoadMesh();

		unsigned int m_VBO = 0;
		unsigned int m_EBO = 0;
		unsigned int m_VAO = 0;

		glm::vec3 m_Position = glm::vec3(0.f);
		glm::vec3 m_Rotation = glm::vec3(0.f);
		glm::vec3 m_Scale = glm::vec3(0.f);

		json JSON;

	};
}
