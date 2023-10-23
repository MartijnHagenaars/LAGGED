#pragma once
#include "entt/entt.hpp"

namespace LAG
{
	class EntityBase
	{
	public:
		EntityBase() = delete;
		EntityBase(const EntityBase&) = delete;
		~EntityBase();

	private:
		friend class Scene;
		EntityBase(entt::entity entityID);


	};
}
