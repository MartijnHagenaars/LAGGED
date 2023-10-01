#pragma once
#include <string>

namespace LAG::ShaderData
{
	struct GLShaderBase
	{
		virtual const std::string& GetVertex() const = 0;
		virtual const std::string& GetPixel() const = 0;
	};
}