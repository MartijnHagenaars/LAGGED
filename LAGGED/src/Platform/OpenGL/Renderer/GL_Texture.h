#pragma once

namespace LAG
{
	class Texture
	{
	public:
		Texture() = delete;
		explicit Texture(std::string path);
		~Texture();

		void Bind();
		void Unbind();

	private:
		bool LoadTexture(const std::string& path);

		unsigned int m_ID = 0; 

		int m_TexWidth = 0;
		int m_TexHeight = 0;
		int m_TexChannels = 0;

	};

}
