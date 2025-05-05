#pragma once
#include <vector>

#include "Buffers/Buffers.h"

namespace tinygltf
{
	class Model;
}

namespace LAG
{
	class Mesh
	{
	public: 
		bool Load(const std::string& path, tinygltf::Model& modelData, size_t meshIndex);
		void Unload();

		void Render();

	private:
		ArrayBuffer m_MeshBuffer;
		std::vector<size_t> m_Textures;

		unsigned int m_TotalIndices = 0;
	};
}
