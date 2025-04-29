#pragma once
#include <glm/vec2.hpp>

namespace LAG
{
	struct PostProcessingProperties
	{
		float m_InversionAmount = 0.f;
		float m_GrayScaleAmount = 0.f;
	};

	class FrameBuffer
	{
	public:
		bool Initialize();
		bool Shutdown();

		void FrameStart(bool showWireframe);
		void FrameEnd();

		void SetPostProcessingProperties(const PostProcessingProperties& postProcessingProps) { m_PostProcessingProperties = postProcessingProps; }

		void* GetEditorHandle();

		void Resize(const glm::uvec2& size);

		const glm::uvec2& GetSize() const;
		void UseWindowSize(bool useWindowSize) { m_UseWindowSize = useWindowSize; }

	protected:
		PostProcessingProperties m_PostProcessingProperties;
		glm::uvec2 m_FrameBufferSize;

		bool m_UseWindowSize = true;

		//FIXME: The variables below need to be moved to a platform-specific structure.
		unsigned int m_VBO = 0;
		unsigned int m_EBO = 0;
		unsigned int m_VAO = 0;

		unsigned int m_FrameBuffer = 0;
		unsigned int m_ColorBuffer = 0;
		unsigned int m_DepthStencilBuffer = 0;
	};
}
