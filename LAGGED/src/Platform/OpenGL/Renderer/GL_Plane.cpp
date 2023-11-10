#include "Precomp.h"
#include "GL_Plane.h"

#include "ECS/Components/BasicComponents.h"
#include "ECS/Components/CameraComponent.h"

#include "GL/glew.h"
#include "Exceptions/GL_GraphicsExceptionMacros.h"

namespace LAG
{
	Plane::Plane(const Utility::String& path) :
		PlaneBase(path)
	{
	}

	Plane::~Plane()
	{
	}

	void Plane::Render(TransformComponent& transform, CameraComponent& camera, Shader& shader)
	{
		glBindVertexArray(m_VAO);

	}

	bool Plane::Load()
	{
		LAG_GRAPHICS_EXCEPTION(glGenVertexArrays(1, &m_VAO));
		LAG_GRAPHICS_EXCEPTION(glGenBuffers(1, &m_VBO));
		LAG_GRAPHICS_EXCEPTION(glGenBuffers(1, &m_EBO));

		float vertices[] = 
		{
		   -0.5f, -0.5f,  0.0f,		0.0f,  0.0f,  1.0f,		0.0f, 0.0f,  //Bottom-left
		   -0.5f,  0.5f,  0.0f,		0.0f,  0.0f,  1.0f,		0.0f, 1.0f,  //Top-left
			0.5f,  0.5f,  0.0f,		0.0f,  0.0f,  1.0f,		1.0f, 1.0f,  //Top-right
			0.5f, -0.5f,  0.0f,		0.0f,  0.0f,  1.0f,		1.0f, 0.0f   //Bottom-right
		};

		unsigned char indices[] = 
		{ 
			0, 1, 2,
			0, 2, 3 
		};

		LAG_GRAPHICS_EXCEPTION(glBindVertexArray(m_VAO));
		LAG_GRAPHICS_EXCEPTION(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
		LAG_GRAPHICS_EXCEPTION(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_DRAW));

		LAG_GRAPHICS_EXCEPTION(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO));
		LAG_GRAPHICS_EXCEPTION(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices[0], GL_STATIC_READ));

		LAG_GRAPHICS_EXCEPTION(glVertexAttribPointer(0, 3, GL_FLOAT, false, 8 * sizeof(float), (void*)0));
		LAG_GRAPHICS_EXCEPTION(glEnableVertexAttribArray(0));
		LAG_GRAPHICS_EXCEPTION(glVertexAttribPointer(1, 3, GL_FLOAT, false, 8 * sizeof(float), (void*)(3 * sizeof(float))));
		LAG_GRAPHICS_EXCEPTION(glEnableVertexAttribArray(1));
		LAG_GRAPHICS_EXCEPTION(glVertexAttribPointer(2, 2, GL_FLOAT, false, 8 * sizeof(float), (void*)(6 * sizeof(float))));
		LAG_GRAPHICS_EXCEPTION(glEnableVertexAttribArray(2));

		LAG_GRAPHICS_EXCEPTION(glBindVertexArray(0));
		//LAG_GRAPHICS_EXCEPTION(glBindBuffer(GL_ARRAY_BUFFER, 0));
		//LAG_GRAPHICS_EXCEPTION(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

		return true;
	}

	bool Plane::Unload()
	{
		LAG_GRAPHICS_EXCEPTION(glBindVertexArray(0));
		LAG_GRAPHICS_EXCEPTION(glBindBuffer(GL_ARRAY_BUFFER, 0));
		LAG_GRAPHICS_EXCEPTION(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

		LAG_GRAPHICS_EXCEPTION(glDeleteBuffers(1, &m_VBO));
		LAG_GRAPHICS_EXCEPTION(glDeleteBuffers(1, &m_EBO));
		LAG_GRAPHICS_EXCEPTION(glDeleteBuffers(1, &m_VAO));

		return true;
	}
}
