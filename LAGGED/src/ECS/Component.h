#pragma once
#include <string>

namespace entt
{
	class meta_type;
}

namespace LAG
{           
	class Entity;
	class Component
	{
	public:
		Component() = delete;
		Component(const entt::meta_type& metaType);

		void AddToEntity(const Entity& entity);
		bool ExistsOnEntity(const Entity& entity);

		std::string GetDisplayName() const { return m_DisplayName; }

	private:
		const entt::meta_type& m_MetaType;
		std::string m_DisplayName;
	};
}