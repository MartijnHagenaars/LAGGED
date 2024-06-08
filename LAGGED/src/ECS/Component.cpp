#include "Component.h"
#include "entt/entt.hpp"

#include "Core/Engine.h"
#include "ECS/Scene.h"

#include "Entity.h"
#include "Meta/ReflectionComponentSetup.h"

namespace LAG
{
	Component::Component(const entt::meta_type& metaType) :
		m_MetaType(metaType)
	{
		entt::meta_prop memberDisplayNameProp = metaType.prop(Reflection::ComponentProperties::DISPLAY_NAME);
		if (memberDisplayNameProp)
			m_DisplayName = memberDisplayNameProp.value().cast<std::string>();
		else
			m_DisplayName = metaType.info().name();
	}

	void Component::AddToEntity(const Entity& entity)
	{
		auto func = m_MetaType.func(entt::hashed_string("ADD_COMPONENT"));
		if (func)
			func.invoke(entt::meta_handle(), entity);
		else
			Logger::Critical("Failed to add component {0} to entity: Meta function not found.", GetDisplayName());
	}

	bool Component::ExistsOnEntity(const Entity& entity)
	{
		auto func = m_MetaType.func(entt::hashed_string("IS_COMPONENT_ON_ENTITY"));
		if (func)
			return func.invoke(entt::meta_handle(), entity).cast<bool>();
		else
			Logger::Critical("Failed to check if component {0} exists on entity: Meta function not found.", GetDisplayName());

		return false;
	}
}
