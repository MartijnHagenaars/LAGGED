#include "Precomp.h"
#include "DX12_Mesh.h"
#include "Platform/WindowBase.h"
#include "DX12_CommandQueue.h"

#include <iostream>
#include <filesystem>

#include "Utility/Logger.h"
#include "Utility/Clamp.h"
       
#pragma comment(lib, "D3DCompiler.lib")

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
		//FIRST, recalculate the MVP. Doing this here, since the Mesh class shouldn't have an update function
		{
			//Update the model matrix
			float modelAngle = static_cast<float>(0.01f * 90.f);
			const DirectX::XMVECTOR modelRotationAxis = DirectX::XMVectorSet(0, 1, 1, 0);
			m_ModelMatrix = DirectX::XMMatrixRotationAxis(modelRotationAxis, DirectX::XMConvertToRadians(modelAngle));

			//Update the view matrix
			const DirectX::XMVECTOR eyePosition = DirectX::XMVectorSet(0, 0, -10, 1);
			const DirectX::XMVECTOR focusPoint = DirectX::XMVectorSet(0, 0, 0, 1);
			const DirectX::XMVECTOR upDirection = DirectX::XMVectorSet(0, 1, 0, 0);
			m_ViewMatrix = DirectX::XMMatrixLookAtLH(eyePosition, focusPoint, upDirection);

			//Update the projection matrix
			float aspectRatio = Window::GetWidth() / static_cast<float>(Window::GetHeight());
			m_ProjMatrix = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(m_FOV), aspectRatio, 0.1f, 100.0f);
		}


		//Now do the actual rendering stuff
		{
			auto commandQueue = Renderer::GetCommandQueue(D3D12_COMMAND_LIST_TYPE_DIRECT); 
			auto commandList = commandQueue->GetCommandList();
			auto currentBackBuffer = Renderer::GetCurrentBackBuffer();
			auto currentRenderTargetView = Renderer::GetCurrentRenderTargetView();
			auto depthCPUDescHandle = m_DSVHeap->GetCPUDescriptorHandleForHeapStart();

			//First, clear the render target
			float clearColor[] = { 0.4f, 0.5f, 0.6f, 1.f };
			TransitionResource(commandList, currentBackBuffer, D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
			ClearRTV(commandList, currentRenderTargetView, clearColor);
			ClearDepth(commandList, depthCPUDescHandle, 1.f);

			//Now we prepare the rendering pipeline for rendering
			commandList->SetPipelineState(m_PipelineState.Get()); //Bind the PSO, which sets all defined shader stages.
			commandList->SetGraphicsRootSignature(m_RootSignature.Get()); //Need to reassign the RS again, otherwise DX12 will complain. 

			//Setup the input assembler. Required, as this tells the input assembler how to interpret the vertex and index data. 
			commandList->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			commandList->IASetVertexBuffers(0, 1, &m_VertexBufferView); //2nd argument set to "1" since we're only using one vertex buffer view
			commandList->IASetIndexBuffer(&m_IndexBufferView);

			//Setup the rasterizer state
			commandList->RSSetViewports(1, &m_Viewport);
			commandList->RSSetScissorRects(1, &m_ScissorRect); //Scissor rect must always be set. Failing to do so might result in a blank screen

			//Binding the render targets for the output merger
			//The render target must be bound to the output merger before drawing
			//Note to self: On 3dgep, it is stated that the "false" argument is something related descriptors to bind are contiguous in memory. WHAT DOES THIS MEAN?!?
			commandList->OMSetRenderTargets(1, &currentRenderTargetView, false, &depthCPUDescHandle);

			//Now, whenever the root signature changes, the arguments that were previously bound to the pipeline need to be rebound. 
			//Any root arguments that've been changed between draw calls also need to be updated. 
			//In this case, the transformation matrix needs to be updated. 
			DirectX::XMMATRIX mvpMatrix = DirectX::XMMatrixMultiply(m_ModelMatrix, m_ViewMatrix);
			mvpMatrix = DirectX::XMMatrixMultiply(mvpMatrix, m_ProjMatrix);
			commandList->SetGraphicsRoot32BitConstants(0, sizeof(DirectX::XMMATRIX) / 4, &mvpMatrix, 0); //Upload the matrix to the GPU


			//Now the rendering pipeline is properly set up, and we can start drawing a cube!

			//Execute the draw method on the command list. This will cause vertices bound to the IA stage to be pushed through the graphics rendering pipeline that's configured above. 
			//The result will be the final rendered geometry being recorded into the render targets bound to the output merger stage.
			commandList->DrawIndexedInstanced(m_Indices.size(), 1, 0, 0, 0);


			//Now present the render!
			TransitionResource(commandList, currentBackBuffer, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);

			m_FenceValues[Renderer::GetCurrentBackBufferIndex()] = commandQueue->ExecuteCommandList(commandList);
			UINT64 newBackBufferIndex = Renderer::Present();
			commandQueue->WaitForFenceValue(m_FenceValues[newBackBufferIndex]);

		}


	}

	bool Mesh::LoadContent()
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

		ComPtr<ID3D12Device5> device = GetDevice();
		std::shared_ptr<DX12_CommandQueue> commandQueue = GetCommandQueue(D3D12_COMMAND_LIST_TYPE_DIRECT);
		ComPtr<ID3D12GraphicsCommandList5> commandList = commandQueue->GetCommandList();


		//Upload the vertex buffer

		//intermediateVertexResource is an intermediate resource used for transferring the vertex buffer data from the CPU to the GPU. 
		//m_VertexBuffer is the destination resource, which is used for rendering the mesh. 
		ComPtr<ID3D12Resource> intermediateVertexResource;
		UpdateBufferResource(commandList, &m_VertexBuffer, &intermediateVertexResource, m_Vertices.size(), sizeof(VertexData), m_Vertices.data());

		m_VertexBufferView.BufferLocation = m_VertexBuffer->GetGPUVirtualAddress();	//Address of the buffer
		m_VertexBufferView.SizeInBytes = sizeof(m_Vertices.data());					//Size (in bytes) of the buffer
		m_VertexBufferView.StrideInBytes = sizeof(VertexData);						//Size (in bytes) if each vertex entry

		
		//Upload the index buffer
		ComPtr<ID3D12Resource> intermediateIndexResource;
		UpdateBufferResource(commandList, &m_IndexBuffer, &intermediateIndexResource, m_Indices.size(), sizeof(unsigned short), m_Indices.data());
		//hello
		m_IndexBufferView.BufferLocation = m_IndexBuffer->GetGPUVirtualAddress();
		m_IndexBufferView.Format = DXGI_FORMAT_R16_UINT;
		m_IndexBufferView.SizeInBytes = sizeof(m_Indices.data());

		m_DSVHeap = CreateDescriptorHeap(GetDevice(), D3D12_DESCRIPTOR_HEAP_TYPE_DSV, false, 1); 

		std::filesystem::path cwd = std::filesystem::current_path();
		std::cout << "Current path is " << std::filesystem::current_path() << '\n'; // (1)
		ComPtr<ID3DBlob> vertexShaderBlob;

		//Utility::Logger::Info("Does vertex shader exist at \"..\\res\\Shaders\\VertexShader.cso\"? : {0}", std::filesystem::exists("..\\res\\Shaders\\VertexShader.cso") ? "yes" : "no");
		LAG_GRAPHICS_EXCEPTION(D3DReadFileToBlob(L"res/Shaders/VertexShader.cso", &vertexShaderBlob));

		ComPtr<ID3DBlob> pixelShaderBlob;
		LAG_GRAPHICS_EXCEPTION(D3DReadFileToBlob(L"res/Shaders/PixelShader.cso", &pixelShaderBlob));

		//Describe the input layout of the shader
		D3D12_INPUT_ELEMENT_DESC inputLayout[] =
		{
			//Semantic name (HLSL semantic associated with this element), semantic index (only needed when there is more than one element of the same semantic), 
			//format (specifies element data format), input slot (identifies the input-assembler or something?), aligned byte offset (Offset in bytes between each element. D3D12_APPEND_ALIGNED_ELEMENT can be used to define the current element after the previous one), 
			//input slot class (Identifies if the input data class is for a single input slot), instance data step rate (Number of instances to draw uisng the same per-instance data (defined by the previous arg) before advancing in the buffer by one element. Should be set to 0 for an element that contains per-vertex data)
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
			{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
		};

		//Now create the root signature
		D3D12_FEATURE_DATA_ROOT_SIGNATURE rootSigVersion = {};
		rootSigVersion.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;

		//If root signature v1.1 is not supported, fall back to version 1.0
		if (device->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE, &rootSigVersion, sizeof(rootSigVersion)) != S_OK)
			rootSigVersion.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_0;

		//Now that we have that set up, we have to define the root signature flags. Here, we want to deny access to certain pipepine stages. 
		//Only the vertex shader currently needs access to the root signature. Denying root signature access to some shader stages results in minor optimizations.
		D3D12_ROOT_SIGNATURE_FLAGS rootSigFlags = 
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS; //Deny the pixel shader access to the root signature

		//The root signature uses a single 32-bit constant parameter (??). We must describe this. 
		CD3DX12_ROOT_PARAMETER1 rootParameters[1];
		//1 - num32BitValues: The number of 32-bit constants. In the case of XMMATRIX, 16 32-bit floating point values are used.
		//2 - Shader register to bind to: in this case, we bind it to b0. 
		//3 - The register space to bind to. Since no shader register space was specified in the vertex shader, it default to space0. 
		//4 - Shader visibility: Specifies the shader stages that are allowed to access the contents at that root signature slot. In this case, the visibility of the constant is restricted to the vertex shader stage. 
		rootParameters[0].InitAsConstants(sizeof(DirectX::XMMATRIX) / 4, 0, 0, D3D12_SHADER_VISIBILITY_VERTEX);

		//After describing the parameters and samplers that are used by the root sig, we now need to create the root signature description. 
		CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSigDesc;
		//1 - Number of root parameters in the root signature
		//2 - The array of parameters
		//3 - Number of static samplers in the root signature (in this case, it's zero)
		//4 - The array of static samplers (here, there is none)
		//5 - Flags that determine the root signature visiblity to the various shader stages.
		// TODO: SHOULD I NOT DO THIS? rootSigDesc.Init_1_1(_countof(rootParameters), &rootParameters[0], 0, nullptr, rootSigFlags);
		rootSigDesc.Init_1_1(_countof(rootParameters), rootParameters, 0, nullptr, rootSigFlags);

		//The final step for creating the root sig is to serialize the root signature description into a binary object that can be used to create the actual root signature. 
		ComPtr<ID3DBlob> rootSignatureBlob;
		ComPtr<ID3DBlob> errorBlob; 
		//First, we need to serialize the root signature. Serializing the root sig description turns it into a binary object that can be used to create the actual signature. 
		LAG_GRAPHICS_EXCEPTION(D3DX12SerializeVersionedRootSignature(&rootSigDesc, rootSigVersion.HighestVersion, &rootSignatureBlob, &errorBlob));
		//Next, create the root signature!
		LAG_GRAPHICS_EXCEPTION(device->CreateRootSignature(0, rootSignatureBlob->GetBufferPointer(), rootSignatureBlob->GetBufferSize(), IID_PPV_ARGS(&m_RootSignature)));




		//Before we can create the PSO (Pipeline state object), the number of render targets and the render target formats need to be defined
		D3D12_RT_FORMAT_ARRAY rtvFormats = {};
		rtvFormats.NumRenderTargets = 1;
		rtvFormats.RTFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;

		//Now we can describe the pipeline state object, which is used to describe how to configure the PSO
		//Here, we apply the properties of the PSO with the various objects that have been described earlier. 
		m_PipelineStateStream = std::make_unique<PipelineStateStream>();
		m_PipelineStateStream->rootSignature = m_RootSignature.Get();
		m_PipelineStateStream->inputLayout = { inputLayout, _countof(inputLayout) };
		m_PipelineStateStream->primitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		m_PipelineStateStream->VS = CD3DX12_SHADER_BYTECODE(vertexShaderBlob.Get());
		m_PipelineStateStream->PS = CD3DX12_SHADER_BYTECODE(pixelShaderBlob.Get());
		m_PipelineStateStream->RTVFormats = rtvFormats;
		m_PipelineStateStream->DSVFormat = DXGI_FORMAT_D32_FLOAT;

		//Now that the pipeline state stream struct is complete, we can create the actual pipeline state object. 
		D3D12_PIPELINE_STATE_STREAM_DESC pssDesc = {};
		pssDesc.SizeInBytes = sizeof(PipelineStateStream);
		pssDesc.pPipelineStateSubobjectStream = &m_PipelineStateStream;

		LAG_GRAPHICS_EXCEPTION(device->CreatePipelineState(&pssDesc, IID_PPV_ARGS(&m_PipelineState)));



		//Before we finish loading the content, the command list must be executed on the command queue to ensure that the index and vertex buffers are uploaded to the GPU resources before rendering
		UINT64 fenceValue = commandQueue->ExecuteCommandList(commandList);
		commandQueue->WaitForFenceValue(fenceValue);

		//TODO: Confirm that this returns the client width and height, and not the non-client window sizes
		ResizeDepthBuffer(Window::GetWidth(), Window::GetHeight());

		m_HasLoadedContent = true;
		return m_HasLoadedContent;
	}

	void Mesh::UnloadContent()
	{
		
	}

	void Mesh::TransitionResource(ComPtr<ID3D12GraphicsCommandList5> commandList, ComPtr<ID3D12Resource> resource, D3D12_RESOURCE_STATES beforeState, D3D12_RESOURCE_STATES afterState)
	{
		//Before a resource can be used, it needs to be in the correct state. Transitioning from one state to another is done using a resource barrier. This can be done by inserting a resource barrier into the command list
		//Needs to be done when, for example, using the swapchains back buffer as a render target. Slso needs to be done before presenting, as it needs to be transitioned to the PRESENT state.

		CD3DX12_RESOURCE_BARRIER resBarrier = CD3DX12_RESOURCE_BARRIER::Transition(resource.Get(), beforeState, afterState);
		commandList->ResourceBarrier(1, &resBarrier);
	}

	void Mesh::ClearRTV(ComPtr<ID3D12GraphicsCommandList5> commandList, D3D12_CPU_DESCRIPTOR_HANDLE rtv, FLOAT* clearColor)
	{
		commandList->ClearRenderTargetView(rtv, clearColor, 0, nullptr); //Using nullptr to clear the entire resource view
	}

	void Mesh::ClearDepth(ComPtr<ID3D12GraphicsCommandList5> commandList, D3D12_CPU_DESCRIPTOR_HANDLE dsv, float depth)
	{
		commandList->ClearDepthStencilView(dsv, D3D12_CLEAR_FLAG_DEPTH, depth, 0, 0, nullptr);
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

	void Mesh::ResizeDepthBuffer(unsigned int width, unsigned int height)
	{
		if (m_HasLoadedContent)
		{
			auto device = Renderer::GetDevice();

			//It is important to the depth buffer is not being references in any command list (while it is being executed on a command list) so we need to flush the command queues. 
			Renderer::FlushCommandQueues();

			auto bufferWidth = Window::GetWidth();
			auto bufferHeight = Window::GetHeight();

			//Depth buffers can never have a size of 0, so we must ensure that it's always valid. 
			if (bufferWidth <= 0)
				bufferWidth = 1;
			if (bufferHeight <= 0)
				bufferHeight = 1;
			
			//Describe the clear value
			D3D12_CLEAR_VALUE clearValue; 
			clearValue.Format = DXGI_FORMAT_D32_FLOAT;
			clearValue.DepthStencil = { 1.f, 0 };

			//Here, we create a depth buffer. Since the depth buffer is a 2D texture, we use the CD3DX12_RESOURCE_DESC::Tex2D helper function to describe what that'll look like. 
			CD3DX12_RESOURCE_DESC tex2D = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_D32_FLOAT, bufferWidth, bufferHeight, 1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);
			CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
			(device->CreateCommittedResource(
				&heapProps,
				D3D12_HEAP_FLAG_NONE,
				&tex2D,
				D3D12_RESOURCE_STATE_DEPTH_WRITE, 
				&clearValue, 
				IID_PPV_ARGS(&m_DepthBuffer)));

			D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc;
			dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
			dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
			dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
			dsvDesc.Texture2D.MipSlice = 0; 

			device->CreateDepthStencilView(m_DepthBuffer.Get(), &dsvDesc, m_DSVHeap->GetCPUDescriptorHandleForHeapStart());

		}
		else Utility::Logger::Warning("Cannot resize depth buffer because content has not yet been loaded.");

	}
}