#include "ModelComponent.h"
#include "ECS/Reflection/SceneReflect.h"

namespace LAG
{
	bool ModelComponent::InitializeReflection()
	{
		SceneReflect& refl = SceneReflect::Get();
		refl.ReflectComponent<ModelComponent>().SetDisplayName("Model Component");

		refl.ReflectVariable(&ModelComponent::modelHandle).SetDisplayName("Model");

		return true;
	}
}
