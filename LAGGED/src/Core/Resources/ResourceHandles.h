#pragma once
#include "Utility/Hash.h"

namespace LAG
{
	struct TextureHandle
	{
		StringHash m_TextureLookup;
	};

	struct ModelHandle
	{
		StringHash m_ModelLookup;
	};
}