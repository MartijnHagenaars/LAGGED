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

		LAG_GRAPHICS_CHECK(device->CreateCommandQueue(&desc, IID_PPV_ARGS(&m_CommandQueue)));
		LAG_GRAPHICS_CHECK(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_Fence)));

		m_FenceEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	}

	DX12_CommandQueue::~DX12_CommandQueue()
	{
	}

	UINT64 DX12_CommandQueue::ExecuteCommandList(ComPtr<ID3D12GraphicsCommandList2> commandList)
	{
		commandList->Close();

		//Get the command allocator
		ID3D12CommandAllocator* commandAllocator;
		UINT dataSize = sizeof(commandAllocator);
		LAG_GRAPHICS_CHECK(commandList->GetPrivateData(__uuidof(ID3D12CommandAllocator), &dataSize, &commandAllocator));

		ID3D12CommandList* const ppCommandLists[] = {
			commandList.Get()
		};

		m_CommandQueue->ExecuteCommandLists(1, ppCommandLists);
		uint64_t fenceValue = Signal();

		//Add all the data to the queues
		m_CommandAllocatorQueue.emplace(CommandAllocatorEntry{ fenceValue, commandAllocator });
		m_CommandListQueue.push(commandList);

		//Since the contents of this ComPtr have been moved to the m_CommandAllocatorQueue, and since it was a temporary ComPtr, we can release it.
		//Calling this releases the interface pointer.
		commandAllocator->Release();
		return fenceValue;
	}

	void DX12_CommandQueue::Flush()
	{
		WaitForFenceValue(Signal());
	}

	ComPtr<ID3D12GraphicsCommandList2> DX12_CommandQueue::GetCommandList()
	{
		ComPtr<ID3D12CommandAllocator> commandAllocator;
		ComPtr<ID3D12GraphicsCommandList2> commandList;

		//First, get a valid command allocator
		bool isAllocatorQueueNotEmpty = !m_CommandAllocatorQueue.empty();
		bool hasCommandAllocatorQueueFenceCompleted = false;
		if (isAllocatorQueueNotEmpty)
			hasCommandAllocatorQueueFenceCompleted = HasFenceCompleted(m_CommandAllocatorQueue.front().fenceValue);

		if (isAllocatorQueueNotEmpty && hasCommandAllocatorQueueFenceCompleted)
		{
			commandAllocator = m_CommandAllocatorQueue.front().commandAllocator;
			m_CommandAllocatorQueue.pop();

			LAG_GRAPHICS_CHECK(commandAllocator->Reset()); //Re-use memory associated with this command alloc
		}
		else
		{
			commandAllocator = CreateCommandAllocator();
		}

		//Next, get a valid command list
		if (!m_CommandListQueue.empty())
		{
			commandList = m_CommandListQueue.front(); 
			m_CommandListQueue.pop();
			LAG_GRAPHICS_CHECK(commandList->Reset(commandAllocator.Get(), nullptr)); //Reset to initial state
		}
		else
		{
			commandList = CreateCommandList(commandAllocator);
		}

		//Associate the command allocator to the private data interface so that we can retrieve the command allocator when executing the command list. 
		LAG_GRAPHICS_CHECK(commandList->SetPrivateDataInterface(__uuidof(ID3D12CommandAllocator), commandAllocator.Get()));
		


		return commandList;
	}

	ComPtr<ID3D12CommandAllocator> DX12_CommandQueue::CreateCommandAllocator()
	{
		ComPtr<ID3D12CommandAllocator> commandAllocator;
		LAG_GRAPHICS_CHECK(m_Device->CreateCommandAllocator(m_CommandListType, IID_PPV_ARGS(&commandAllocator)));

		return commandAllocator;
	}

	ComPtr<ID3D12GraphicsCommandList2> DX12_CommandQueue::CreateCommandList(ComPtr<ID3D12CommandAllocator> commandAllocator)
	{
		ComPtr<ID3D12GraphicsCommandList2> commandList;
		LAG_GRAPHICS_CHECK(m_Device->CreateCommandList(0, m_CommandListType, commandAllocator.Get(), nullptr, IID_PPV_ARGS(&commandList)));
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
	}
}