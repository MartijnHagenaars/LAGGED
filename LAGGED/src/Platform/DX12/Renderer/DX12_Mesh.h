#pragma once
#include "Core/Defines.h"

//!! Contents of this function are temporarily hardcoded to work for DX12. Will be changed after I figure out how to do this !!
#include "DX12_Common.h"
#include "DX12_Renderer.h"

#include "Utility/Timer.h"

namespace LAG::Renderer
{
	struct PipelineStateStream
	{
		CD3DX12_PIPELINE_STATE_STREAM_ROOT_SIGNATURE rootSignature; //Wrapper for pointer to ID3D12RootSignature
		CD3DX12_PIPELINE_STATE_STREAM_INPUT_LAYOUT inputLayout; //Wrapper for D3D12_INPUT_LAYOUT_DESC, which specifies the input layout for the input assembly stage
		CD3DX12_PIPELINE_STATE_STREAM_PRIMITIVE_TOPOLOGY primitiveTopologyType; //Wrapper for D3D12_PRIMITIVE_TOPOLOGY_TYPE, which specifies the primitive topology type (point, line triangle, etc)
		CD3DX12_PIPELINE_STATE_STREAM_VS VS; //Wrapper for D3D12_SHADER_BYTECODE, which specifies the compiled VS
		CD3DX12_PIPELINE_STATE_STREAM_PS PS; //Wrapper for D3D12_SHADER_BTYECODE, which specifies the PS
		CD3DX12_PIPELINE_STATE_STREAM_DEPTH_STENCIL_FORMAT DSVFormat; //Wrapper for DXGI_FORMAT enum, which describes the format of the depth stencil buffer
		CD3DX12_PIPELINE_STATE_STREAM_RENDER_TARGET_FORMATS RTVFormats; //Wrapper for D3D12_RT_FORMAT_ARRAY struct, which is a wrapper for an array of render target formats
	};

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

		LAG_API bool LoadContent(); 
		LAG_API void UnloadContent();

		void TransitionResource(ComPtr<ID3D12GraphicsCommandList2> commandList, ComPtr<ID3D12Resource> resource, D3D12_RESOURCE_STATES beforeState, D3D12_RESOURCE_STATES afterState);

		void ClearRTV(ComPtr<ID3D12GraphicsCommandList2> commandList, D3D12_CPU_DESCRIPTOR_HANDLE rtv, FLOAT* clearColor);
		void ClearDepth(ComPtr<ID3D12GraphicsCommandList2> commandList, D3D12_CPU_DESCRIPTOR_HANDLE dsv, float depth = 1.f);

		//Create a resource that is large enough to hold buffer data, as well as creating an intermediate upload buffer that sticks around till the command list is finished uploaded the resource to the destination buffer on the GPU.
		//commandList: Required for transfering buffer data to the destination resource
		//destinationResource & intermediateResource: Used to store the destination and intermediate resources that are created in this function
		//numElements, elementSize & bufferData: Information about the CPU buffer that is passed to the GPU resource
		void UpdateBufferResource(ComPtr<ID3D12GraphicsCommandList2> commandList, ID3D12Resource** destinationResource, ID3D12Resource** intermediateResource,
			size_t numElements, size_t elementSize, const void* bufferData, D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_NONE);

		//Resize the depth buffer to the same size as the window
		void ResizeDepthBuffer(unsigned int width, unsigned int height);

		//These variables should be private, but currently changing them in the renderer class. Don't ask why. 
		D3D12_VIEWPORT m_Viewport;
		D3D12_RECT m_ScissorRect;

	private: 
		std::vector<VertexData> m_Vertices;
		std::vector<unsigned short> m_Indices;

		uint64_t m_FenceValues[3] = {}; //TODO: FUCKING FIX THIS HARDCODED NONSENSE. 
		//std::vector<UINT64> m_FenceValues; //TODO: Change this to a std::array at some point. 

		ComPtr<ID3D12Resource> m_VertexBuffer;
		ComPtr<ID3D12Resource> m_IndexBuffer;

		D3D12_VERTEX_BUFFER_VIEW m_VertexBufferView;
		D3D12_INDEX_BUFFER_VIEW m_IndexBufferView;

		ComPtr<ID3D12Resource> m_DepthBuffer;
		ComPtr<ID3D12DescriptorHeap> m_DSVHeap; //The depth buffer requires a depth stencil view, which is created in this descriptor heap. 

		ComPtr<ID3D12RootSignature> m_RootSignature;
		ComPtr<ID3D12PipelineState> m_PipelineState;

		bool m_HasLoadedContent = false;
		float m_FOV = 45.f; //NEEDS TO BE MOVED TO A CAMERA CLASS
		
		DirectX::SimpleMath::Matrix m_ModelMatrix;
		DirectX::SimpleMath::Matrix m_ViewMatrix;
		DirectX::SimpleMath::Matrix m_ProjMatrix;

		Utility::Timer m_ObjectLifetime;

	};
}