#pragma once
#include "Platform/Base/Renderer/FrameBuffer.h"

namespace LAG
{
	class FrameBuffer : public FrameBufferBase
	{
	public:
		FrameBuffer();
		~FrameBuffer() override;

		void FrameStart(bool showWireframe) override;
		void FrameEnd() override;

	private:
		bool Initialize();
		bool Shutdown();

		unsigned int m_VBO = 0;
		unsigned int m_EBO = 0;
		unsigned int m_VAO = 0; 

		unsigned int m_FrameBuffer = 0;
		unsigned int m_ColorBuffer = 0;
		unsigned int m_DepthStencilBuffer = 0;
	};
}


