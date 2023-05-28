#pragma once
#include "Platform/RendererBase.h"
#include "DX12_Common.h"
#include "DX12_CommandQueue.h"

namespace LAG::Renderer
{
	class DX12_DescriptorHeap;

	//Returns a ComPtr to the device
	ComPtr<ID3D12Device5> GetDevice();
	std::shared_ptr<DX12_CommandQueue> GetCommandQueue(D3D12_COMMAND_LIST_TYPE type);

	ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeap(ComPtr<ID3D12Device5> device, D3D12_DESCRIPTOR_HEAP_TYPE type, bool shaderVisible, UINT32 totalDescriptors);

	const UINT64 GetTotalSwapChains();
}

