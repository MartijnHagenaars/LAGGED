#pragma once
#include "Core/Defines.h"
#include "WindowBase.h"

namespace LAG::Renderer
{
	// ===== Essential render functions =====

	LAG_API bool Initialize();
	LAG_API bool Shutdown();

	LAG_API void StartFrame();
	LAG_API void EndFrame();

	LAG_API void Render();
	LAG_API void Clear();

	LAG_API void SetPipelineState(int pipelineID);

	//// ===== Render surface functions =====

	//class Surface
	//{
	//public:
	//	Surface() = default; 
	//	virtual ~Surface() {};

	//	virtual void Render() = 0; 
	//	virtual void Resize(unsigned int width, unsigned int height) = 0;

	//	virtual unsigned int GetWidth() const = 0;
	//	virtual unsigned int GetHeight() const = 0;
	//	
	//	int GetID() const { return m_SurfaceID; }

	//protected:
	//	int m_SurfaceID = -1;
	//};

	//std::unique_ptr<Surface> CreateSurface(std::unique_ptr<Window>& window);
	//std::unique_ptr<Surface> GetSurface(unsigned int index);
	//void RemoveSurface(unsigned int index); 
	//void ResizeSurface(unsigned int index, unsigned int width, unsigned int height);
	//void RenderSurface(unsigned int index);

}
