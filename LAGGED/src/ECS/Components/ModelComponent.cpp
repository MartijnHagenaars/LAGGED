#include "ModelComponent.h"
#include "ECS/Reflection/SceneReflect.h"

namespace LAG
{
	bool ModelComponent::InitializeReflection()
	{
		SceneReflect& refl = SceneReflect::Get();
		refl.RegisterComponent<ModelComponent>().SetDisplayName("Model Component");

		refl.RegisterVariable(&ModelComponent::modelHandle).SetDisplayName("Model");

		return true;
	}
}
