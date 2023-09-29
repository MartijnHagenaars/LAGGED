#pragma once
#include "Core/Resources/Resource.h"

namespace LAG
{
	class Texture : public Resource
	{
		friend class ResourceManager;
		friend class Model;
	public:
		Texture() = delete;
		explicit Texture(const Utility::String& path);
		~Texture() override;

		void Bind(size_t textureUnit);
		void Unbind(size_t textureUnit);

	private:
		bool Load() override;
		bool Unload() override;

		unsigned int m_ID = 0; 

		int m_TexWidth = 0;
		int m_TexHeight = 0;
		int m_TexChannels = 0;

	};

}
