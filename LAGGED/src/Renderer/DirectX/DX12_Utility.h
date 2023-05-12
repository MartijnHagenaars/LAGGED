#pragma once
#include "Renderer/DirectX/DX12_Common.h"

namespace LAG::Renderer
{
	//Converts an std::string to a "Long Pointer to Constant Wide String". 
	LPCWSTR ConvertStringToLPCWSTR(std::string input);

	std::string ConvertCommandListTypeToString(D3D12_COMMAND_LIST_TYPE type);
}