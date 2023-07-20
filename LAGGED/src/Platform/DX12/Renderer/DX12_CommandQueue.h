#pragma once
#include <queue>

namespace LAG::Renderer
{
	enum class CommandQueueType
	{
		Direct = 0,
		Copy,
		Compute
	};

	class DX12_CommandQueue
	{
	public:
		DX12_CommandQueue(ComPtr<ID3D12Device5> device, D3D12_COMMAND_LIST_TYPE type);
		~DX12_CommandQueue();

		//Executes the assigned command list. 
		//Returns a fence value, which can be used to wait for the executing command list.
		UINT64 ExecuteCommandList(ComPtr<ID3D12GraphicsCommandList2> commandList);

		UINT64 Signal();
		void WaitForFenceValue(UINT64 fenceValue);
		bool HasFenceCompleted(UINT64 fenceValue);

		//Reset the command queue to its initial state
		void Flush();

		ComPtr<ID3D12GraphicsCommandList2> GetCommandList();
		ComPtr<ID3D12CommandQueue> GetCommandQueue() const { return m_CommandQueue; }

	private:
		ComPtr<ID3D12CommandAllocator> CreateCommandAllocator();
		ComPtr<ID3D12GraphicsCommandList2> CreateCommandList(ComPtr<ID3D12CommandAllocator> commandAllocator);

	private:

		ComPtr<ID3D12Device5> m_Device;
		ComPtr<ID3D12CommandQueue> m_CommandQueue = nullptr;
		D3D12_COMMAND_LIST_TYPE m_CommandListType; //Stores what the list type is (Direct, copy, compute, etc.)

		//Synchronization primitives
		ComPtr<ID3D12Fence1> m_Fence = nullptr;
		HANDLE m_FenceEvent;
		UINT64 m_FenceValue = 0; 

		//Command allocator and list queues
		//Since the command allocator cannot be reused immediately, we need to ensure that it's no longer in-flight by signaling a fence value. 
		struct CommandAllocatorEntry
		{
			UINT64 fenceValue = 0;
			ComPtr<ID3D12CommandAllocator> commandAllocator;
		};
		std::queue<CommandAllocatorEntry> m_CommandAllocatorQueue;
		std::queue<ComPtr<ID3D12GraphicsCommandList2>> m_CommandListQueue;

	};
}


