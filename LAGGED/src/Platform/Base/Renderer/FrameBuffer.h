#pragma once
#include "glm/vec2.hpp"

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

		virtual void* GetEditorHandle() = 0;

		virtual void Resize(const glm::uvec2& size) = 0;

		virtual const glm::uvec2& GetSize() const = 0;
		void UseWindowSize(bool useWindowSize) { m_UseWindowSize = useWindowSize; }

	protected:
		PostProcessingProperties m_PostProcessingProperties;
		bool m_UseWindowSize = true;
		glm::uvec2 m_FrameBufferSize;
	};
}

//Include the correct model header, based on the project configuration
#ifdef PLATFORM_OPENGL
#include "Platform/OpenGL/Renderer/GL_FrameBuffer.h"
#elif PLATFORM_DIRECTX

#endif
