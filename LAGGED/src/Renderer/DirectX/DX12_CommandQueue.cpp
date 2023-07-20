#include "Precomp.h"
#include "DX12_Common.h"
#include "DX12_CommandQueue.h"
#include <chrono>

namespace LAG::Renderer
{
	DX12_CommandQueue::DX12_CommandQueue(ComPtr<ID3D12Device5> device, D3D12_COMMAND_LIST_TYPE type) :
		m_CommandListType(type), m_Device(device)
	{
		D3D12_COMMAND_QUEUE_DESC desc;
		desc.Type = type;
		desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
		desc.NodeMask = 0;
		desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

		LAG_GRAPHICS_EXCEPTION(device->CreateCommandQueue(&desc, IID_PPV_ARGS(&m_CommandQueue)));
		LAG_GRAPHICS_EXCEPTION(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_Fence)));

		m_FenceEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
		assert(m_FenceEvent && "Failed to create fence event handle.");
	}

	DX12_CommandQueue::~DX12_CommandQueue()
	{
	}

	UINT64 DX12_CommandQueue::ExecuteCommandList(ComPtr<ID3D12GraphicsCommandList2> commandList)
	{
		////The recording state has finished, so close the list. 
		//commandList->Close();

		////Get the command allocator
		////ComPtr<ID3D12CommandAllocator> commandAllocator = nullptr;
		////UINT commandAllocDataSize = sizeof(ID3D12CommandAllocator*);
		//ID3D12CommandAllocator* commandAllocator = nullptr;
		//UINT commandAllocDataSize = sizeof(commandAllocator);

		//HRESULT hresGetterShit = commandList->GetPrivateData(__uuidof(ID3D12CommandAllocator), &commandAllocDataSize, &commandAllocator);

		////Now create an array of lists for executing...
		//ID3D12CommandList* const lists[] = { 
		//	commandList.Get() 
		//};

		//m_CommandQueue->ExecuteCommandLists(1, lists);
		//UINT64 fenceValue = Signal(); //Get the fence value so that we wait for the command allocator to be ready for reuse. 

		////Add all the data to the queues
		//m_CommandAllocatorQueue.push(CommandAllocatorEntry({ fenceValue, commandAllocator}));
		//m_CommandListQueue.push(commandList);

		////Since the contents of this ComPtr have been moved to the m_CommandAllocatorQueue, and since it was a temporary ComPtr, we can release it.
		////Calling this releases the interface pointer.
		//HRESULT hresFucker = commandAllocator->Release();
		//LAG_GRAPHICS_EXCEPTION(hresFucker);
		//LAG_GRAPHICS_EXCEPTION_PREV();

		//commandAllocator->SetName(L"Dead cunt");
		//return fenceValue;

		commandList->Close();

		ID3D12CommandAllocator* commandAllocator;
		UINT dataSize = sizeof(commandAllocator);
		LAG_GRAPHICS_EXCEPTION(commandList->GetPrivateData(__uuidof(ID3D12CommandAllocator), &dataSize, &commandAllocator));

		ID3D12CommandList* const ppCommandLists[] = {
			commandList.Get()
		};

		m_CommandQueue->ExecuteCommandLists(1, ppCommandLists);
		uint64_t fenceValue = Signal();

		m_CommandAllocatorQueue.emplace(CommandAllocatorEntry{ fenceValue, commandAllocator });
		printf("Executing command allocator...\n");
		m_CommandListQueue.push(commandList);

		// The ownership of the command allocator has been transferred to the ComPtr
		// in the command allocator queue. It is safe to release the reference 
		// in this temporary COM pointer here.
		commandAllocator->Release();
		return fenceValue;
	}

	void DX12_CommandQueue::Flush()
	{
		/*UINT64 fenceValueForSignal = Signal();
		HasFenceCompleted(fenceValueForSignal);*/

		WaitForFenceValue(Signal());
	}

	ComPtr<ID3D12GraphicsCommandList2> DX12_CommandQueue::GetCommandList()
	{
		ComPtr<ID3D12CommandAllocator> commandAllocator;
		ComPtr<ID3D12GraphicsCommandList2> commandList;
		
		int kutzooi = m_CommandAllocatorQueue.size();
		printf("");

		//First, get a valid command allocator
		bool isAllocatorQueueNotEmpty = !m_CommandAllocatorQueue.empty();
		bool hasCommandAllocatorQueueFenceCompleted = false;
		if (isAllocatorQueueNotEmpty)
			hasCommandAllocatorQueueFenceCompleted = HasFenceCompleted(m_CommandAllocatorQueue.front().fenceValue);

		if (isAllocatorQueueNotEmpty && hasCommandAllocatorQueueFenceCompleted)
		{
			commandAllocator = m_CommandAllocatorQueue.front().commandAllocator;
			m_CommandAllocatorQueue.pop();

			LAG_GRAPHICS_EXCEPTION(commandAllocator->Reset()); //Re-use memory associated with this command alloc

			printf("Getting command allocator...\n");
		}
		else
		{
			commandAllocator = CreateCommandAllocator();
			printf("Creating command allocator...\n");
		}

		//Next, get a valid command list
		if (!m_CommandListQueue.empty())
		{
			commandList = m_CommandListQueue.front(); 
			m_CommandListQueue.pop();
			LAG_GRAPHICS_EXCEPTION(commandList->Reset(commandAllocator.Get(), nullptr)); //Reset to initial state
		}
		else
		{
			commandList = CreateCommandList(commandAllocator);
		}

		//Associate the command allocator to the private data interface so that we can retrieve the command allocator when executing the command list. 
		LAG_GRAPHICS_EXCEPTION(commandList->SetPrivateDataInterface(__uuidof(ID3D12CommandAllocator), commandAllocator.Get()));
		


		return commandList;
	}

	ComPtr<ID3D12CommandAllocator> DX12_CommandQueue::CreateCommandAllocator()
	{
		ComPtr<ID3D12CommandAllocator> commandAllocator;
		LAG_GRAPHICS_EXCEPTION(m_Device->CreateCommandAllocator(m_CommandListType, IID_PPV_ARGS(&commandAllocator)));

		return commandAllocator;
	}

	ComPtr<ID3D12GraphicsCommandList2> DX12_CommandQueue::CreateCommandList(ComPtr<ID3D12CommandAllocator> commandAllocator)
	{
		ComPtr<ID3D12GraphicsCommandList2> commandList;
		LAG_GRAPHICS_EXCEPTION(m_Device->CreateCommandList(0, m_CommandListType, commandAllocator.Get(), nullptr, IID_PPV_ARGS(&commandList)));
		return commandList;
	}


	UINT64 DX12_CommandQueue::Signal()
	{
		UINT64 fenceValue = ++m_FenceValue;
		m_CommandQueue->Signal(m_Fence.Get(), fenceValue);
		return fenceValue;
	}

	void DX12_CommandQueue::WaitForFenceValue(UINT64 fenceValue)
	{
		//Wait if the completed value is lower than the m_FenceValue
		if (!HasFenceCompleted(fenceValue))
		{
			m_Fence->SetEventOnCompletion(fenceValue, m_FenceEvent);
			::WaitForSingleObject(m_FenceEvent, static_cast<DWORD>(std::chrono::milliseconds::max().count()));
		}
	}

	bool DX12_CommandQueue::HasFenceCompleted(UINT64 fenceValue)
	{
		return m_Fence->GetCompletedValue() >= fenceValue;
		//return m_Fence->GetCompletedValue() >= m_FenceValue;
	}
}