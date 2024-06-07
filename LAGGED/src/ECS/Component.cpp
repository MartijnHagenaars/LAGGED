#include "Component.h"
#include "entt/entt.hpp"

#include "Meta/ReflectionComponentSetup.h"

namespace LAG
{
	Component::Component(entt::meta_type& metaType) :
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

	}
}
