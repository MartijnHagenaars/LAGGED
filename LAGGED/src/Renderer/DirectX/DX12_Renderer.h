#pragma once
#include "Platform/RendererBase.h"
#include "DX12_Common.h"
#include "DX12_CommandQueue.h"

namespace LAG::Renderer
{
	class DX12_DescriptorHeap;

	//Returns a ComPtr to the device
	ComPtr<ID3D12Device5> GetDevice();
	ComPtr<ID3D12CommandQueue> GetCommandQueue();


	const UINT64 GetTotalSwapChains();
}

