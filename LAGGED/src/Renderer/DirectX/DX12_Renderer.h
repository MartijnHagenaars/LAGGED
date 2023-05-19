#pragma once
#include "Platform/RendererBase.h"
#include "DX12_Common.h"
#include <mutex> //MOVE THIS
#include <array> //SAME FOR THIS

namespace LAG::Renderer
{
	class DX12_DescriptorHeap;

	//Returns a ComPtr to the device
	ComPtr<ID3D12Device5> GetDevice();
}

