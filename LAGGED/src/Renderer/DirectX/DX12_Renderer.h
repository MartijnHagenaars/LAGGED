#pragma once
#include "Platform/RendererBase.h"
#include "DX12_Common.h"
#include <mutex> //MOVE THIS
#include <array> //SAME FOR THIS

namespace LAG::Renderer
{
	class DX12_DescriptorHeap;

	//Returns a ComPtr to the device
	ComPtr<ID3D12Device5> GetDevice();


	//Temporary functions: 

	LAG_API void LoadContent();
	LAG_API void UnloadContent();

	void TransitionResource(ComPtr<ID3D12GraphicsCommandList7> commandList, ComPtr<ID3D12Resource> resource, D3D12_RESOURCE_STATES beforeState, D3D12_RESOURCE_STATES afterState);

	void ClearRTV(ComPtr<ID3D12GraphicsCommandList7> commandList, D3D12_CPU_DESCRIPTOR_HANDLE rtv, FLOAT* clearColor);
	void ClearDepth(ComPtr<ID3D12GraphicsCommandList7> commandList, D3D12_CPU_DESCRIPTOR_HANDLE dsv, float depth = 1.f);

	void UpdateBufferResource(ComPtr<ID3D12GraphicsCommandList7> commandList, ID3D12Resource** destinationResource, ID3D12Resource** intermediateResource,
		size_t numElements, size_t elementSize, const void* bufferData, D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_NONE);

	void ResizeDepthBuffer(int width, int height);
}

