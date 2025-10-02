#pragma once
#include "Utility/Hash.h"

namespace LAG
{
	struct TextureHandle
	{
		StringHash64 m_TextureLookup;
	};

	struct ModelHandle
	{
		StringHash64 m_ModelLookup;
	};
}