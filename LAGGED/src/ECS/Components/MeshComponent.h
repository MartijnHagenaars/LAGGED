#pragma once
#include "Utility/String.h"

namespace LAG
{
	struct MeshComponent
	{
		MeshComponent(std::string path) : 
			meshPath(Utility::String(path.c_str()))
		{
		}

		Utility::String meshPath;
	};
}