#pragma once
#include "Core/Resources/Resource.h"

namespace LAG
{
	class Texture : public Resource
	{
		friend class ResourceManager;
	public:
		explicit Texture(const Utility::String& path);
		~Texture() override;

		void Bind();
		void Unbind();

	private:
		Texture() = default;
		bool Load() override;
		bool Unload() override;

		unsigned int m_ID = 0; 

		int m_TexWidth = 0;
		int m_TexHeight = 0;
		int m_TexChannels = 0;

	};

}
