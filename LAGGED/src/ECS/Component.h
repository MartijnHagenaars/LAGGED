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
		friend class Scene;
	public:
		Component() = delete;

		void AddToEntity(const Entity& entity);

		std::string GetDisplayName() const { return m_DisplayName; }

	private:
		Component(entt::meta_type& metaType);
		
		entt::meta_type& m_MetaType;
		std::string m_DisplayName;
	};
}