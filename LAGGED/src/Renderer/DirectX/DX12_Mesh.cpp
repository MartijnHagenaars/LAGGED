#include "Precomp.h"
#include "Platform/MeshBase.h"

namespace LAG
{
	Mesh::Mesh()
	{

	}

	Mesh::~Mesh()
	{

	}

	void Mesh::Render()
	{
	}

	LAG_API void Mesh::LoadContent()
	{
		
	}

	LAG_API void Mesh::UnloadContent()
	{
		
	}

	void Mesh::TransitionResource(ComPtr<ID3D12GraphicsCommandList7> commandList, ComPtr<ID3D12Resource> resource, D3D12_RESOURCE_STATES beforeState, D3D12_RESOURCE_STATES afterState)
	{
	}

	void Mesh::ClearRTV(ComPtr<ID3D12GraphicsCommandList7> commandList, D3D12_CPU_DESCRIPTOR_HANDLE rtv, FLOAT* clearColor)
	{
	}

	void Mesh::ClearDepth(ComPtr<ID3D12GraphicsCommandList7> commandList, D3D12_CPU_DESCRIPTOR_HANDLE dsv, float depth)
	{
	}

	void Mesh::UpdateBufferResource(ComPtr<ID3D12GraphicsCommandList7> commandList, ID3D12Resource** destinationResource, ID3D12Resource** intermediateResource, size_t numElements, size_t elementSize, const void* bufferData, D3D12_RESOURCE_FLAGS flags)
	{
	}

	void Mesh::ResizeDepthBuffer(int width, int height)
	{
	}
}