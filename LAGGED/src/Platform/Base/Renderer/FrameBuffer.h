#pragma once

namespace LAG
{
	struct PostProcessingProperties
	{
		float m_InversionAmount = 0.f;
		float m_GrayScaleAmount = 0.f;
	};

	class FrameBufferBase
	{
	public:
		FrameBufferBase() = default;
		virtual ~FrameBufferBase() {};

		virtual void FrameStart(bool showWireframe) = 0;
		virtual void FrameEnd() = 0;

		virtual void SetPostProcessingProperties(const PostProcessingProperties& postProcessingProps) { m_PostProcessingProperties = postProcessingProps; }

	protected:
		PostProcessingProperties m_PostProcessingProperties;
	};
}

//Include the correct model header, based on the project configuration
#ifdef PLATFORM_OPENGL
#include "Platform/OpenGL/Renderer/GL_FrameBuffer.h"
#elif PLATFORM_DIRECTX
#include "Platform/DX12/Renderer/DX12_FrameBuffer.h"
#endif
