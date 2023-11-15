#pragma once
#include "Utility/HashedString.h"

namespace LAG
{
	struct MeshComponent
	{
		MeshComponent(std::string path) : 
			meshPath(HashedString(path.c_str()))
		{
		}

		HashedString meshPath;
	};
}