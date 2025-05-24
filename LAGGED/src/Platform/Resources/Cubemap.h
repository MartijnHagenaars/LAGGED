#pragma once
#include "Core/Resources/Resource.h"
#include "Platform/Defines.h"

namespace LAG
{
	class Cubemap : public Resource
	{
		friend class Model;
		friend class ResourceManager;
	public:
		Cubemap(const HashedString& path);
		~Cubemap() = default;

		bool Load() override;
		bool Unload() override;


		void Bind();
		void Unbind();

	private:
		void* m_DataPtr = nullptr;
	};
}