#include "Precomp.h"
#include "DX12_Renderer.h"
#include "DX12_Common.h"

//Used to retrieve the WIN32Data struct
#include "Window/WIN32/WIN32_Window.h"

#include <array>
#include <mutex>

//TODO: REMOVE THIS!
#include "DX12_Mesh.h"

using namespace LAG::Utility;
//#define LAG::Utility::Logger Logger;

namespace LAG::Renderer
{
	struct RendererData
	{
		ComPtr<IDXGIFactory7> factory = nullptr;
		ComPtr<IDXGIAdapter4> adapter = nullptr;
		ComPtr<ID3D12Device5> device = nullptr;

		ComPtr<IDXGISwapChain4> swapChain = nullptr;
		static const UINT64 totalSwapChainBackBuffers = 3; //Not very good
		ComPtr<ID3D12Resource> swapChainBackBuffers[totalSwapChainBackBuffers];

		UINT64 fenceValues[totalSwapChainBackBuffers];
		std::shared_ptr<DX12_CommandQueue> directCommandQueue = nullptr;
		std::shared_ptr<DX12_CommandQueue> copyCommandQueue = nullptr;
		std::shared_ptr<DX12_CommandQueue> computeCommandQueue = nullptr;

		UINT rtvDescSize = 0;
		ComPtr<ID3D12DescriptorHeap> RTVDescHeap = nullptr; //Temp location

		D3D_FEATURE_LEVEL minFeatureLevel = D3D_FEATURE_LEVEL_11_0;
		D3D_FEATURE_LEVEL maxFeatureLevel; 

		BOOL useVSync = true;
		BOOL isTearingSupported = false;

		UINT64 currentBackBufferIndex = 0; 

		//TODO: THIS IS TEMPORARY. REMOVE SOON.
		Mesh* tempMesh = nullptr;
	};
	std::unique_ptr<RendererData> renderData = nullptr;

	//Creates an adapter 
	ComPtr<IDXGIAdapter4> CreateDXGIAdapter(ComPtr<IDXGIFactory7> dxgiFactory)
	{
		ComPtr<IDXGIAdapter4> dxgiAdapter = nullptr; 
		//Loop through all adapters to find the highest performing one. 
		for (UINT i = 0; DXGI_ERROR_NOT_FOUND != dxgiFactory->EnumAdapterByGpuPreference(i, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&dxgiAdapter)); i++)
		{
			//Check if adapter supports the minimum feature level
			if (SUCCEEDED(D3D12CreateDevice(dxgiAdapter.Get(), renderData->minFeatureLevel, __uuidof(ID3D12Device), nullptr)))
			{
				return dxgiAdapter;
			}
			else
			{
				if(dxgiAdapter)
					dxgiAdapter->Release();
				dxgiAdapter = nullptr;
			}
		}

		return nullptr;
	}

	ComPtr<ID3D12Device5> CreateDevice(ComPtr<IDXGIAdapter4> dxgiAdapter)
	{
		//First, find the maximum feature level
		const D3D_FEATURE_LEVEL featureLevels[5]
		{
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_12_0,
			D3D_FEATURE_LEVEL_12_1,
			D3D_FEATURE_LEVEL_12_2
		};

		D3D12_FEATURE_DATA_FEATURE_LEVELS featureLevelData = { 0 };
		featureLevelData.NumFeatureLevels = sizeof(featureLevels);
		featureLevelData.pFeatureLevelsRequested = featureLevels;

		//Create a temporary device to get the maximum feature level.
		{
			ComPtr<ID3D12Device> tempDevice;
			LAG_GRAPHICS_EXCEPTION(D3D12CreateDevice(dxgiAdapter.Get(), renderData->minFeatureLevel, IID_PPV_ARGS(&tempDevice)));
			LAG_GRAPHICS_EXCEPTION(tempDevice->CheckFeatureSupport(D3D12_FEATURE_FEATURE_LEVELS, &featureLevelData, sizeof(featureLevelData)));
			renderData->maxFeatureLevel = featureLevelData.MaxSupportedFeatureLevel;

			//Confirm that the max supported feature level is valid
			if (renderData->maxFeatureLevel < renderData->minFeatureLevel)
			{
				Logger::Error("Max feature level is lower than min feature level\n");
				LAG_GRAPHICS_EXCEPTION_PREV();
				return nullptr; 
			}

			if(tempDevice != nullptr)
				tempDevice.Reset();
		}

		//Once that is done, create the device
		ComPtr<ID3D12Device5> device = nullptr;
		LAG_GRAPHICS_EXCEPTION(D3D12CreateDevice(dxgiAdapter.Get(), renderData->maxFeatureLevel, IID_PPV_ARGS(&device)));
		LAG_SET_D3D12_NAME(device, "Core_Device");
		return device;
	}

	//Returns a ComPtr of a command queue. 
	//The command queue is used to submit command lists to be executed.
	ComPtr<ID3D12CommandQueue> CreateCommandQueue(ComPtr<ID3D12Device5> device, D3D12_COMMAND_LIST_TYPE type)
	{
		ComPtr<ID3D12CommandQueue> commandQueue;
		
		D3D12_COMMAND_QUEUE_DESC desc = { };
		desc.Type = type;
		desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
		desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		desc.NodeMask = 0; 

		LAG_GRAPHICS_EXCEPTION(device->CreateCommandQueue(&desc, IID_PPV_ARGS(&commandQueue)));
		return commandQueue;
	}

	//Weird
	bool CheckTearingSupport()
	{
		BOOL allowTearing = FALSE;

		// Rather than create the DXGI 1.5 factory interface directly, we create the
		// DXGI 1.4 interface and query for the 1.5 interface. This is to enable the 
		// graphics debugging tools which will not support the 1.5 factory interface 
		// until a future update.
		ComPtr<IDXGIFactory4> factory4;
		if (SUCCEEDED(CreateDXGIFactory1(IID_PPV_ARGS(&factory4))))
		{
			ComPtr<IDXGIFactory5> factory5;
			if (SUCCEEDED(factory4.As(&factory5)))
			{
				if (FAILED(factory5->CheckFeatureSupport(
					DXGI_FEATURE_PRESENT_ALLOW_TEARING,
					&allowTearing, sizeof(allowTearing))))
				{
					allowTearing = FALSE;
				}
			}
		}

		return allowTearing == TRUE;
	}

	ComPtr<IDXGISwapChain4> CreateSwapChain(std::shared_ptr<DX12_CommandQueue> directCommandQueue, ComPtr<IDXGIFactory7> factory, UINT32 bufferCount)
	{
		ComPtr<IDXGISwapChain4> swapChain;

		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
		swapChainDesc.Width = Window::GetWidth();
		swapChainDesc.Height = Window::GetHeight();
		swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
		swapChainDesc.Stereo = false;
		swapChainDesc.SampleDesc = { 1, 0 };
		swapChainDesc.BufferCount = bufferCount;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
		swapChainDesc.Flags = CheckTearingSupport() ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;

		DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullScreenDesc = {};
		fullScreenDesc.Windowed = true;

		ComPtr<IDXGISwapChain1> swapChainTemp;
		LAG_GRAPHICS_EXCEPTION(factory->CreateSwapChainForHwnd(directCommandQueue->GetCommandQueue().Get(), static_cast<const LAG::Window::WIN32Data*>(Window::GetWindowData())->hWnd, &swapChainDesc, &fullScreenDesc, nullptr, &swapChainTemp));
		LAG_GRAPHICS_EXCEPTION(swapChainTemp.As(&swapChain));

		return swapChain;
	}

	//Returns a ComPtr containing a descriptor heap
	//A descriptor heap can be considered an array of resource views (such as RTVs, SRVs, UAVs, CBVs, etc.)
	ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeap(ComPtr<ID3D12Device5> device, D3D12_DESCRIPTOR_HEAP_TYPE type, bool shaderVisible, UINT32 totalDescriptors)
	{
		ComPtr<ID3D12DescriptorHeap> descriptorHeap;

		D3D12_DESCRIPTOR_HEAP_DESC desc = {};
		desc.NumDescriptors = totalDescriptors;
		desc.Type = type;
		desc.NodeMask = 0; 

		//D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE only applies to CBV, SRV, UAV and samplers. Because of that, always disable it for other desc. heap types. 
		if (type != D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
			shaderVisible = false; 
		desc.Flags = shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

		LAG_GRAPHICS_EXCEPTION(device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&descriptorHeap)));

		return descriptorHeap;
	}

	void UpdateRenderTargetViews(ComPtr<ID3D12Device5> device, ComPtr<IDXGISwapChain4> swapChain, ComPtr<ID3D12DescriptorHeap> descHeap)
	{
		UINT descHeapSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV); //Get the size of a single descriptor. Should not be hardcoded.
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(descHeap->GetCPUDescriptorHandleForHeapStart()); //In order to iterate the descriptors in a descriptor heap, a handle to the first descriptor in the heap is retrieved. 

		int totalLoops = static_cast<int>(renderData->totalSwapChainBackBuffers);
		for (int i = 0; i < totalLoops; i++)
		{
			ComPtr<ID3D12Resource2> backBuffer;
			LAG_GRAPHICS_EXCEPTION(swapChain->GetBuffer(i, IID_PPV_ARGS(&backBuffer))); //Get a pointer to the swapchain's backbuffer

			//Create the RTV. 
			//First parameter is the pointer to the resource.
			//Second parameter is a pointer to a D3D12_RENDER_TARGET_VIEW_DESC. Here, it's set to nullptr since the resource's internal description is used to create the RTV. 
			//Third parameter is the handle to the descriptor where the view is placed. 
			device->CreateRenderTargetView(backBuffer.Get(), nullptr, rtvHandle); 
			LAG_GRAPHICS_EXCEPTION_PREV();

			//Now, we store the resource so that it can be transitioned to the correct state that'll be shown later. 
			renderData->swapChainBackBuffers[i] = backBuffer;

			//Increment the descriptor handle to the next handle in the descriptor heap. 
			rtvHandle.Offset(descHeapSize);
		}
	}

	//Returns a ComPtr containing a ID3D12CommandAllocator
	//The command allocator is the backing memory used by a command list. Does not provide any functionality and can only be accessed indirectly through a command list.
	//A command allocator can only be used by a single command list at a time but can be reused after the commands that were recorded into the command list have finished executing on the GPU.
	//In order to achieve maximum frames, one command allocator per in-flight command list should be created. 
	ComPtr<ID3D12CommandAllocator> CreateCommandAllocator(ComPtr<ID3D12Device5> device, D3D12_COMMAND_LIST_TYPE type)
	{
		ComPtr<ID3D12CommandAllocator> commandAllocator; 
		LAG_GRAPHICS_EXCEPTION(device->CreateCommandAllocator(type, IID_PPV_ARGS(&commandAllocator)));
		return commandAllocator;
	}

	ComPtr<ID3D12GraphicsCommandList5> CreateCommandList(ComPtr<ID3D12Device5> device, ComPtr<ID3D12CommandAllocator> commandAllocator, D3D12_COMMAND_LIST_TYPE type)
	{
		ComPtr<ID3D12GraphicsCommandList5> commandList;

		LAG_GRAPHICS_EXCEPTION(device->CreateCommandList(0, type, commandAllocator.Get(), nullptr, IID_PPV_ARGS(&commandList)));
		
		LAG_GRAPHICS_EXCEPTION(commandList->Close()); //Close the commandList so that it can be reset at the start of the render loop
		return commandList;
	}

	ComPtr<ID3D12Fence1> CreateFence(ComPtr<ID3D12Device5> device)
	{
		ComPtr<ID3D12Fence1> fence;
		D3D12_FENCE_FLAGS flags = {};

		flags |= D3D12_FENCE_FLAG_NONE;
		LAG_GRAPHICS_EXCEPTION(device->CreateFence(0, flags, IID_PPV_ARGS(&fence)));

		return fence;
	}

	//Signal the fence from the GPU.
	//The fence is only signaled once the GPU command queue has reached that point during execution, so any commands that have been queued before the signal method was invoked must complete execution before the fence will be signaled.
	UINT64 SignalFence(ComPtr<ID3D12Fence1> fence, ComPtr<ID3D12CommandQueue> commandQueue, UINT64& fenceValue)
	{
		UINT64 fenceValueForSignal = ++fenceValue;
		commandQueue->Signal(fence.Get(), fenceValueForSignal);
		return fenceValueForSignal;
	}


	//Stall the CPU thread to wait for the GPU queue to finish executing commands that write to resources before being reused. 
	//Example: before reusing a swapchain's backbuffer resource, any commands that are using that resource as a render target must be complete before that back buffer resource can be reused. 
	// 
	//	Note:	Any resources that are never used as a writeable target (for example; material textures) do noit need to be double buffered and do not require stalling the CPU thread before being reused as read-only resources in a shader. 
	//			Writable resource such as render targets do need to be synchronized to protect the resource from being modified by multiple queues at the same time.
	void WaitForFenceValue(ComPtr<ID3D12Fence> fence, UINT64 fenceValue, HANDLE fenceEventHandle, std::chrono::milliseconds waitDuration = std::chrono::milliseconds::max())
	{
		//If the fence has not yet reached the fence value, an event object is registered with the fence and is in turn signaled once the fence has reached the specified value. 
		if (fence->GetCompletedValue() < fenceValue)
		{
			fence->SetEventOnCompletion(fenceValue, fenceEventHandle);
			::WaitForSingleObject(fenceEventHandle, static_cast<DWORD>(waitDuration.count()));
		}
	}

	//Returns an event handle
	//The Event Handle is used to block the CPU thread. 
	HANDLE CreateEventHandle()
	{
		HANDLE fenceEvent;
		fenceEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
		assert(fenceEvent && "Failed to create a fence event");
		return fenceEvent; 
	}

	bool Initialize()
	{
		if (!DirectX::XMVerifyCPUSupport())
		{
			Logger::Critical("XMVerifyCPUSupport returned false: No DirectXMath support.");
			return false;
		}

		if (renderData == nullptr)
			renderData = std::make_unique<RendererData>();
		else Logger::Error("Pointer to render struct already exists.");

#ifdef DEBUG
		ComPtr<ID3D12Debug> debugInterface;
		LAG_GRAPHICS_EXCEPTION(D3D12GetDebugInterface(IID_PPV_ARGS(&debugInterface)));
		debugInterface->EnableDebugLayer();
#endif

		//Initialize the exception info queue
		InfoQueueManager::Initialize();

		//Create the factory
		UINT dxgiFactoryFlags = 0; 
#ifdef DEBUG
		//Enable debugging data when running in debug
		dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif
		LAG_GRAPHICS_EXCEPTION(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&renderData->factory))); //Same thing as above, just shortened using a macro

		//Create the adapter and device
		renderData->adapter = CreateDXGIAdapter(renderData->factory);
		renderData->device = CreateDevice(renderData->adapter);

		renderData->directCommandQueue = std::make_shared<DX12_CommandQueue>(renderData->device, D3D12_COMMAND_LIST_TYPE_DIRECT);
		renderData->copyCommandQueue = std::make_shared<DX12_CommandQueue>(renderData->device, D3D12_COMMAND_LIST_TYPE_COPY);
		renderData->computeCommandQueue = std::make_shared<DX12_CommandQueue>(renderData->device, D3D12_COMMAND_LIST_TYPE_COMPUTE);

		renderData->swapChain = CreateSwapChain(renderData->directCommandQueue, renderData->factory, renderData->totalSwapChainBackBuffers);
		renderData->currentBackBufferIndex = renderData->swapChain->GetCurrentBackBufferIndex();

		renderData->RTVDescHeap = CreateDescriptorHeap(renderData->device, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, renderData->totalSwapChainBackBuffers);
		renderData->rtvDescSize = renderData->device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

		UpdateRenderTargetViews(renderData->device, renderData->swapChain, renderData->RTVDescHeap);

		renderData->tempMesh = new Mesh();
		renderData->tempMesh->LoadContent();

		//Ensure that the RTVs are off the right size. 
		OnResize();

		return true;
	}

	void FlushCommandQueues()
	{
		renderData->directCommandQueue->Flush();
		renderData->copyCommandQueue->Flush();
		renderData->computeCommandQueue->Flush();
	}

	bool Shutdown()
	{
		FlushCommandQueues();

		renderData->device.Reset();
		renderData->adapter.Reset();
		renderData->factory.Reset();

		InfoQueueManager::Shutdown();

		if (renderData != nullptr)
			renderData.reset();

		return true;
	}

	void StartFrame()
	{

	}

	void EndFrame()
	{

	}

	void Render()
	{
		ComPtr<ID3D12Resource> backBuffer = renderData->swapChainBackBuffers[renderData->currentBackBufferIndex];
		ComPtr<ID3D12GraphicsCommandList5> directCommandList = renderData->directCommandQueue->GetCommandList();
		UINT64 currentBackBufferIndex = renderData->currentBackBufferIndex;

		Mesh* meshRef = renderData->tempMesh;
		meshRef->Render();

		//Old rendering method. Remove when done. 
		{
			////First, clear the render target
			//{
			//	//Before the render target can be cleared, it must be in the RENDER_TARGET state. 
			//	//Change the resource state of the backbuffer.
			//	CD3DX12_RESOURCE_BARRIER resBarrier = CD3DX12_RESOURCE_BARRIER::Transition(backBuffer.Get(), 
			//		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET); //Left is the before state, right is the after state. 

			//	//If there is more than a single resource barrier to insert into the command list, it is recommended to store all barriers in a list and 
			//	// execute them all at the same time before an operation that requires the resource to be in a particular state is executed. 
			//	//In this case, there is only one barrier.
			//	directCommandList->ResourceBarrier(1, &resBarrier);
			//	
			//	FLOAT clearColor[] = { 0.4f, 0.6f, 0.9f, 1.0f };

			//	auto fucker = renderData->RTVDescHeap->GetCPUDescriptorHandleForHeapStart();

			//	//No clue how this works... 
			//	CD3DX12_CPU_DESCRIPTOR_HANDLE rtv(renderData->RTVDescHeap->GetCPUDescriptorHandleForHeapStart(),
			//		renderData->currentBackBufferIndex, renderData->rtvDescSize);

			//	//Clear the render target view and use the clear color
			//	directCommandList->ClearRenderTargetView(rtv, clearColor, 0, nullptr);
			//}

			////Now we can present stuff!
			//{
			//	//First, we need to transition to the present state.
			//	CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
			//		backBuffer.Get(),
			//		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
			//	directCommandList->ResourceBarrier(1, &barrier);


			//	////The command list must always be closed before being executed on the command queue
			//	//LAG_GRAPHICS_EXCEPTION(directCommandList->Close());



			//	renderData->fenceValues[currentBackBufferIndex] = renderData->directCommandQueue->ExecuteCommandList(directCommandList);

			//	UINT syncInterval = renderData->useVSync ? 1 : 0;
			//	UINT presentFlags = renderData->isTearingSupported && !renderData->useVSync ? DXGI_PRESENT_ALLOW_TEARING : 0;
			//	LAG_GRAPHICS_EXCEPTION(renderData->swapChain->Present(syncInterval, presentFlags));
			//	renderData->currentBackBufferIndex = renderData->swapChain->GetCurrentBackBufferIndex();
			//	
			//	renderData->directCommandQueue->WaitForFenceValue(renderData->fenceValues[currentBackBufferIndex]);

			//	//renderData->frameFenceValues[renderData->currentBackBufferIndex] = SignalFence(renderData->fence, renderData->commandQueue, renderData->fenceValue);
			//}
		}


	}

	void Clear()
	{

	}

	void OnResize()
	{
		FlushCommandQueues();

		for (int i = 0; i < renderData->totalSwapChainBackBuffers; ++i)
		{
			// Any references to the back buffers must be released
			// before the swap chain can be resized.
			renderData->swapChainBackBuffers[i].Reset();
			//renderData->frameFenceValues[i] = renderData->frameFenceValues[renderData->currentBackBufferIndex];
		}

		DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
		LAG_GRAPHICS_EXCEPTION(renderData->swapChain->GetDesc(&swapChainDesc));
		LAG_GRAPHICS_EXCEPTION(renderData->swapChain->ResizeBuffers(static_cast<UINT>(renderData->totalSwapChainBackBuffers), Window::GetWidth(), Window::GetHeight(),
			swapChainDesc.BufferDesc.Format, swapChainDesc.Flags));
		LAG_GRAPHICS_EXCEPTION_PREV();

		renderData->currentBackBufferIndex = renderData->swapChain->GetCurrentBackBufferIndex();
		renderData->tempMesh->m_Viewport = CD3DX12_VIEWPORT(0.f, 0.f, FLOAT(Window::GetWidth()), FLOAT(Window::GetHeight()));
		
		UpdateRenderTargetViews(renderData->device, renderData->swapChain, renderData->RTVDescHeap);
	}

	void SetPipelineState(int pipelineID)
	{

	}

	UINT64 Present()
	{
		
		UINT syncInterval = renderData->useVSync ? 1 : 0;
		UINT presentFlags = renderData->isTearingSupported && !renderData->useVSync ? DXGI_PRESENT_ALLOW_TEARING : 0;
		LAG_GRAPHICS_EXCEPTION(renderData->swapChain->Present(syncInterval, presentFlags));
		renderData->currentBackBufferIndex = renderData->swapChain->GetCurrentBackBufferIndex();

		return renderData->currentBackBufferIndex;
	}

	ComPtr<ID3D12Device5> GetDevice()
	{ 
		return renderData->device; 
	}

	std::shared_ptr<DX12_CommandQueue> GetCommandQueue(D3D12_COMMAND_LIST_TYPE type)
	{
		switch (type)
		{
		case D3D12_COMMAND_LIST_TYPE_DIRECT:
			return renderData->directCommandQueue;
		case D3D12_COMMAND_LIST_TYPE_COPY:
			return renderData->copyCommandQueue;
		case D3D12_COMMAND_LIST_TYPE_COMPUTE:
			return renderData->computeCommandQueue;
		default: 
			return std::shared_ptr<DX12_CommandQueue>();
		}
	}

	const UINT64 GetTotalSwapChains()
	{
		return renderData->totalSwapChainBackBuffers;
	}

	D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentRenderTargetView()
	{
		return CD3DX12_CPU_DESCRIPTOR_HANDLE(renderData->RTVDescHeap->GetCPUDescriptorHandleForHeapStart(), renderData->currentBackBufferIndex, renderData->rtvDescSize);
	}

	Microsoft::WRL::ComPtr<ID3D12Resource> GetCurrentBackBuffer()
	{
		return renderData->swapChainBackBuffers[renderData->currentBackBufferIndex];
	}

	UINT64 GetCurrentBackBufferIndex()
	{
		return renderData->currentBackBufferIndex;
	}
}