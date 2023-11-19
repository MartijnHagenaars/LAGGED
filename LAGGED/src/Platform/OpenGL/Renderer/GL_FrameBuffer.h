#pragma once
namespace LAG
{
	class FrameBuffer
	{
	public:
		FrameBuffer();
		~FrameBuffer();

		void FrameStart(bool showWireframe);
		void FrameEnd();

		void DrawPostProcessWindow();

		void PostProcessInversion(float inversionAmount) { m_InversionAmount = inversionAmount; };
		void PostProcessGrayScale(float grayScaleAmount) { m_GrayScaleAmount = grayScaleAmount; }


	private:
		bool Initialize();
		bool Shutdown();

		unsigned int m_VBO = 0;
		unsigned int m_EBO = 0;
		unsigned int m_VAO = 0; 

		unsigned int m_FrameBuffer = 0;
		unsigned int m_ColorBuffer = 0;
		unsigned int m_DepthStencilBuffer = 0;

		float m_InversionAmount = 0.f;
		float m_GrayScaleAmount = 0.f;
	};
}


