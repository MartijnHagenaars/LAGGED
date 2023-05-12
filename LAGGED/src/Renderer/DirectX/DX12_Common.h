#pragma once

//Standard DirectX includes
#include <d3d12.h>
#include <dxgi1_6.h> //DirectX Graphics Infrastructure
#include "d3dx12.h"

//COM includes
#include <wrl.h>
using Microsoft::WRL::ComPtr;

//Debugging / Exception includes
#include "Renderer/DirectX/Exceptions/DX12_ExceptionMacros.h"
#include "Renderer/DirectX/Exceptions/DX12_InfoQueueManager.h"

//Additional includes
#include "Renderer/DirectX/DX12_Utility.h"
#include "Utility/Logger.h"


//TODO: Set the input libs in the project properties (instead of doing it here) 
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d12.lib")

#ifdef DEBUG
#define LAG_SET_D3D12_NAME(object, name) object->SetName(ConvertStringToLPCWSTR(name)); LAG::Utility::Logger::Info("Assigned name to D3D12 object: {0}", name);
#else
#define LAG_SET_D3D12_NAME(object, name) 
#endif

namespace LAG::Renderer
{
	constexpr UINT32 FRAME_BUFFER_COUNT = 3;
	constexpr DXGI_FORMAT DEFAULT_RENDER_TARGET_FORMAT = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
}