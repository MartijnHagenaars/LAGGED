#include "Precomp.h"
#include "DX12_Mesh.h"
#include "Platform/WindowBase.h"

namespace LAG::Renderer
{
	Mesh::Mesh()
	{
		m_FenceValues.reserve(Renderer::GetTotalSwapChains());
		
		//Uesd for making out a rectangular region of th screen which'll allow for rendering. We just want to cover the whole screen so we set it to the max.
		m_ScissorRect = CD3DX12_RECT(0, 0, LONG_MAX, LONG_MAX);
		m_Viewport = CD3DX12_VIEWPORT(0.f, 0.f, FLOAT(Window::GetWidth()), FLOAT(Window::GetHeight()));
	}

	Mesh::~Mesh()
	{

	}

	void Mesh::Render()
	{
	}

	LAG_API void Mesh::LoadContent()
	{
		//Load vertex and index data
		{
			VertexData vertices[8] =
			{
				{ DirectX::SimpleMath::Vector3(-1.0f, -1.0f, -1.0f), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f) }, // 0
				{ DirectX::SimpleMath::Vector3(-1.0f,  1.0f, -1.0f), DirectX::SimpleMath::Vector3(0.0f, 1.0f, 0.0f) }, // 1
				{ DirectX::SimpleMath::Vector3(1.0f,  1.0f, -1.0f), DirectX::SimpleMath::Vector3(1.0f, 1.0f, 0.0f) }, // 2
				{ DirectX::SimpleMath::Vector3(1.0f, -1.0f, -1.0f), DirectX::SimpleMath::Vector3(1.0f, 0.0f, 0.0f) }, // 3
				{ DirectX::SimpleMath::Vector3(-1.0f, -1.0f,  1.0f), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 1.0f) }, // 4
				{ DirectX::SimpleMath::Vector3(-1.0f,  1.0f,  1.0f), DirectX::SimpleMath::Vector3(0.0f, 1.0f, 1.0f) }, // 5
				{ DirectX::SimpleMath::Vector3(1.0f,  1.0f,  1.0f), DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f) }, // 6
				{ DirectX::SimpleMath::Vector3(1.0f, -1.0f,  1.0f), DirectX::SimpleMath::Vector3(1.0f, 0.0f, 1.0f) }  // 7
			};

			unsigned short indicies[36] =
			{
				0, 1, 2, 0, 2, 3,
				4, 6, 5, 4, 7, 6,
				4, 5, 1, 4, 1, 0,
				3, 2, 6, 3, 6, 7,
				1, 5, 6, 1, 6, 2,
				4, 0, 3, 4, 3, 7
			};

			//Insert vertex data
			m_Vertices.clear();
			m_Vertices.insert(m_Vertices.end(), &vertices[0], &vertices[sizeof(vertices) / sizeof(VertexData)]);

			m_Indices.clear();
			m_Indices.insert(m_Indices.end(), &indicies[0], &indicies[sizeof(indicies) / sizeof(unsigned short)]);
		}

		auto device = GetDevice();
		//auto commandQueue = GetCommandQueue
	}

	LAG_API void Mesh::UnloadContent()
	{
		
	}

	void Mesh::TransitionResource(ComPtr<ID3D12GraphicsCommandList5> commandList, ComPtr<ID3D12Resource> resource, D3D12_RESOURCE_STATES beforeState, D3D12_RESOURCE_STATES afterState)
	{
	}

	void Mesh::ClearRTV(ComPtr<ID3D12GraphicsCommandList5> commandList, D3D12_CPU_DESCRIPTOR_HANDLE rtv, FLOAT* clearColor)
	{
	}

	void Mesh::ClearDepth(ComPtr<ID3D12GraphicsCommandList5> commandList, D3D12_CPU_DESCRIPTOR_HANDLE dsv, float depth)
	{
	}

	void Mesh::UpdateBufferResource(ComPtr<ID3D12GraphicsCommandList5> commandList, 
		ID3D12Resource** destinationResource, ID3D12Resource** intermediateResource, 
		size_t numElements, size_t elementSize, const void* bufferData, D3D12_RESOURCE_FLAGS flags)
	{
		auto device = GetDevice();
		size_t bufferSize = numElements * elementSize;

		{
			D3D12_HEAP_PROPERTIES heapProps = {};
			heapProps.Type = D3D12_HEAP_TYPE_DEFAULT;

			D3D12_HEAP_FLAGS heapFlags = {};
			heapFlags |= D3D12_HEAP_FLAG_NONE;

			const D3D12_RESOURCE_DESC resDesc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize, flags);

			//Create a resource and implicit heap that is large enough to store the resource. Resource is also mapped to the implicit heap
			//Argument 1 and 2 are pretty self explanatory: just provide properties and flags for the heap
			//Argument 3: 
			//Argument 4: 
			LAG_GRAPHICS_EXCEPTION(device->CreateCommittedResource(
				&heapProps,
				heapFlags,
				&resDesc,	//Create a structure that describes the buffer resource
				D3D12_RESOURCE_STATE_COPY_DEST,						//Set the initial state of the buffer resource. In this case, the resource is created with a readback heap type
				nullptr,											//Describes the default value for a clear color. Must be NULL when using D3D12_RESOURCE_DIMENSION_BUFFER
				IID_PPV_ARGS(destinationResource)
			));
		}

		

		//Nowthat the resource has been created, the buffer data needs to be uploaded to the resource. 
		//For this, we create another resource that transfers CPU buffer data into GPU memory. To do this, an intermediate buffer resource is created using an upload heap. 

		//Create another committed resource for the data upload
		if (bufferData != nullptr)
		{
			const D3D12_HEAP_PROPERTIES heapDesc = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
			const D3D12_RESOURCE_DESC resDesc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

			LAG_GRAPHICS_EXCEPTION(device->CreateCommittedResource(
				&heapDesc,
				D3D12_HEAP_FLAG_NONE, 
				&resDesc,
				D3D12_RESOURCE_STATE_GENERIC_READ, 
				nullptr, 
				IID_PPV_ARGS(intermediateResource)
			));

			//Now that the two resources have been created, we can transfer CPU buffer data to the GPU resources

			//Describe the subresource that us uploaded to the GPU resource
			D3D12_SUBRESOURCE_DATA subResData = {};
			subResData.pData = bufferData;
			subResData.RowPitch = bufferSize;
			subResData.SlicePitch = subResData.RowPitch;

			//Upload CPU buffer data to the GPU resource in a default heap using an intermediate buffer in an upload heap... Woah!
			UpdateSubresources(
				commandList.Get(), 
				*destinationResource,	//Destination resource
				*intermediateResource,	//Intermediate resource
				0,						//Intermediate resource offset: Offset in bytes to the intermediate resource.
				0,						//NumSubresources: The number of subresources in the resource to be updated. For buffer resources, this is always 1 (or 0, but then this function does nothing_
				1,						//FirstSubresource: First index of the first subresource in the resource. For buffer resources, this is always 0. This is because buffers only have a single subresource at index 0. 
				&subResData
			);

		}

	}

	void Mesh::ResizeDepthBuffer(int width, int height)
	{
	}
}