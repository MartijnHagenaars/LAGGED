#pragma once
#include "Core/Defines.h"

//!! Contents of this function are temporarily hardcoded to work for DX12. Will be changed after I figure out how to do this !!
#include "Renderer/DirectX/DX12_Common.h"

namespace LAG
{
	class Mesh
	{
	public: 
		Mesh();
		~Mesh(); 

		void Render();

		LAG_API void LoadContent(); 
		LAG_API void UnloadContent();

		void TransitionResource(ComPtr<ID3D12GraphicsCommandList7> commandList, ComPtr<ID3D12Resource> resource, D3D12_RESOURCE_STATES beforeState, D3D12_RESOURCE_STATES afterState);

		void ClearRTV(ComPtr<ID3D12GraphicsCommandList7> commandList, D3D12_CPU_DESCRIPTOR_HANDLE rtv, FLOAT* clearColor);
		void ClearDepth(ComPtr<ID3D12GraphicsCommandList7> commandList, D3D12_CPU_DESCRIPTOR_HANDLE dsv, float depth = 1.f);

		void UpdateBufferResource(ComPtr<ID3D12GraphicsCommandList7> commandList, ID3D12Resource** destinationResource, ID3D12Resource** intermediateResource,
			size_t numElements, size_t elementSize, const void* bufferData, D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_NONE);

		void ResizeDepthBuffer(int width, int height);

	};
}