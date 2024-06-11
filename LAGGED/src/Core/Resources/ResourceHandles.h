#pragma once
#include "Utility/HashedString.h"

namespace LAG
{
	struct TextureHandle
	{
		HashedString m_TextureLookup;
	};

	struct ModelHandle
	{
		HashedString m_ModelLookup;
	};
}