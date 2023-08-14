#include "Precomp.h"
#include "GL_Mesh.h"

#include "TinyGLTF/tiny_gltf.h"

namespace LAG
{
	Mesh::Mesh(const std::string& path)
	{
		LoadMesh();
	}

	Mesh::~Mesh()
	{

	}

	bool Mesh::LoadMesh()
	{
		return false;
	}

	void LAG::Mesh::Render()
	{

	}

	void LAG::Mesh::SetPosition()
	{
	}

	void LAG::Mesh::SetRotation()
	{
	}

	void LAG::Mesh::SetScale()
	{
	}
}
