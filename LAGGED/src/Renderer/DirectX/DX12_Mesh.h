#pragma once
#include "Core/Defines.h"

//!! Contents of this function are temporarily hardcoded to work for DX12. Will be changed after I figure out how to do this !!
#include "Renderer/DirectX/DX12_Common.h"

#include "Renderer/DirectX/DX12_Renderer.h"

namespace LAG::Renderer
{
	struct VertexData
	{
		DirectX::SimpleMath::Vector3 Position;
		DirectX::SimpleMath::Vector3 Color;
	};

	class Mesh
	{
	public: 
		Mesh();
		~Mesh(); 

		void Render();

		LAG_API void LoadContent(); 
		LAG_API void UnloadContent();

		void TransitionResource(ComPtr<ID3D12GraphicsCommandList5> commandList, ComPtr<ID3D12Resource> resource, D3D12_RESOURCE_STATES beforeState, D3D12_RESOURCE_STATES afterState);

		void ClearRTV(ComPtr<ID3D12GraphicsCommandList5> commandList, D3D12_CPU_DESCRIPTOR_HANDLE rtv, FLOAT* clearColor);
		void ClearDepth(ComPtr<ID3D12GraphicsCommandList5> commandList, D3D12_CPU_DESCRIPTOR_HANDLE dsv, float depth = 1.f);

		//Create a resource that is large enough to hold buffer data, as well as creating an intermediate upload buffer that sticks around till the command list is finished uploaded the resource to the destination buffer on the GPU.
		//commandList: Required for transfering buffer data to the destination resource
		//destinationResource & intermediateResource: Used to store the destination and intermediate resources that are created in this function
		//numElements, elementSize & bufferData: Information about the CPU buffer that is passed to the GPU resource
		void UpdateBufferResource(ComPtr<ID3D12GraphicsCommandList5> commandList, ID3D12Resource** destinationResource, ID3D12Resource** intermediateResource,
			size_t numElements, size_t elementSize, const void* bufferData, D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_NONE);

		//Resize the depth buffer to the same size as the window
		void ResizeDepthBuffer(int width, int height);

	private: 
		std::vector<VertexData> m_Vertices;
		std::vector<unsigned short> m_Indices;

		std::vector<UINT64> m_FenceValues; //TODO: Change this to a std::array at some point. 

		ComPtr<ID3D12Resource> m_VertexBuffer;
		ComPtr<ID3D12Resource> m_IndexBuffer;

		D3D12_VERTEX_BUFFER_VIEW m_VertexBufferView;
		D3D12_INDEX_BUFFER_VIEW m_IndexBufferView;

		ComPtr<ID3D12Resource> m_DepthBuffer;
		ComPtr<ID3D12DescriptorHeap> m_DSVHeap; //The depth buffer requires a depth stencil view, which is created in this descriptor heap. 

		ComPtr<ID3D12RootSignature> m_RootSignature;
		ComPtr<ID3D12PipelineState> m_PipelineState;

		D3D12_VIEWPORT m_Viewport;
		D3D12_RECT m_ScissorRect;

		float m_FOV = 45.f; //NEEDS TO BE MOVED TO A CAMERA CLASS
		
		DirectX::SimpleMath::Matrix m_ModelMatrix;
		DirectX::SimpleMath::Matrix m_ViewMatrix;
		DirectX::SimpleMath::Matrix m_ProjMatrix;

	};
}