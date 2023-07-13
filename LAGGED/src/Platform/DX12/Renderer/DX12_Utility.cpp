#include "Precomp.h"
#include "DX12_Utility.h"

LPCWSTR LAG::Renderer::ConvertStringToLPCWSTR(std::string input)
{
	std::wstring wStrTemp = std::wstring(input.begin(), input.end());
	return wStrTemp.c_str();
}

std::string LAG::Renderer::ConvertCommandListTypeToString(D3D12_COMMAND_LIST_TYPE type)
{
	switch (type)
	{
	case D3D12_COMMAND_LIST_TYPE_DIRECT:
		return std::string("Direct_Command");
	case D3D12_COMMAND_LIST_TYPE_COPY:
		return std::string("Copy_Command");
	case D3D12_COMMAND_LIST_TYPE_COMPUTE:
		return std::string("Compute_Command");
	default:
		return std::string("Unkown_Command");
	}
}
