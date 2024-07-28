#pragma once
#include <vector>
#include <string>

#include "Platform/Base/Renderer/Buffer.h"

namespace tinygltf
{
	class Model;
	//struct Mesh;
}

namespace LAG
{
	class Mesh
	{
	public: 
		Mesh() = default;


		bool Load(const std::string& path, tinygltf::Model& modelData, size_t meshIndex);
		void Unload();

		void Render();

	private:
		ArrayBuffer m_Buffer;
		std::vector<size_t> m_Textures;

		unsigned int m_TotalIndices = 0;
	};
}
