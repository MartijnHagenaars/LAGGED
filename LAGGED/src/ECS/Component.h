#pragma once
#include <string>

// NOTE: CONSIDER REMOVING CLASS.

namespace LAG
{           
	class Entity;
	class Component
	{
	public:
		Component() = default;

		void AddToEntity(const Entity& entity);
		bool ExistsOnEntity(const Entity& entity);

		std::string GetDisplayName() const { return m_DisplayName; }

	private:
		std::string m_DisplayName;
	};
}