#include "Component.h"
#include "entt/entt.hpp"

#include "Core/Engine.h"
#include "ECS/Scene.h"

#include "Entity.h"
#include "Meta/ReflectionComponentSetup.h"

namespace LAG
{
	using namespace Reflection;
	Component::Component(const entt::meta_type& metaType) :
		m_MetaType(metaType)
	{
		entt::meta_prop memberDisplayNameProp = metaType.prop(ComponentProperties::DISPLAY_NAME);
		if (memberDisplayNameProp)
			m_DisplayName = memberDisplayNameProp.value().cast<std::string>();
		else
			m_DisplayName = metaType.info().name();
	}

	void Component::AddToEntity(const Entity& entity)
	{
		auto func = m_MetaType.func(ComponentProperties::Internal::ADD_COMPONENT);
		if (func)
			func.invoke(entt::meta_handle(), entity);
		else
			CRITICAL("Failed to add component {} to entity: Meta function not found.", GetDisplayName());
	}

	bool Component::ExistsOnEntity(const Entity& entity)
	{
		auto func = m_MetaType.func(entt::hashed_string(ComponentProperties::Internal::IS_COMPONENT_ON_ENTITY));
		if (func)
			return func.invoke(entt::meta_handle(), entity).cast<bool>();
		else
			CRITICAL("Failed to check if component {} exists on entity: Meta function not found.", GetDisplayName());

		return false;
	}
}
