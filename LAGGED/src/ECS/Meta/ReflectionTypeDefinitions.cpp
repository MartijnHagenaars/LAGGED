#include "ReflectionTypeDefinitions.h"

namespace LAG
{
	namespace Reflection
	{
		int InitializeReflectionTypes()
		{
			SetupReflectionType<int>();
			SetupReflectionType<bool>();
			SetupReflectionType<float>();
			SetupReflectionType<std::string>();

			SetupReflectionType<glm::vec3>();

			SetupReflectionType<LAG::TextureHandle>();
			SetupReflectionType<LAG::NoiseData>();

			return true;
		}
	}
}
