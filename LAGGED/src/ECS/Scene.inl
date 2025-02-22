#include "Scene.h"

namespace LAG
{
	template<typename Comp, typename ...Args>
	inline Comp* Scene::AddComponent(const EntityID entityID, Args && ...args)
	{
		ComponentData* compData = nullptr;
		const ComponentID compID = GetComponentID<Comp>();

		const auto& compDataIt = m_ComponentMap.find(compID);
		if (compDataIt == m_ComponentMap.end())
		{
			compData = RegisterComponent<Comp>();
			INFO("Component {} not registered. Registering with ID {}...", typeid(Comp).name(), compID);
		}
		else compData = compDataIt->second;

		return nullptr;
	}

	template<typename Comp>
	inline const ComponentID Scene::GetComponentID()
	{
		static const ComponentID compID = s_ComponentCounter++;
		return compID;
	}

	template<typename Comp>
	inline Scene::ComponentData* Scene::RegisterComponent()
	{
		ComponentID compID = GetComponentID<Comp>();

		//Check if component has already been registered. 
		const auto& compPtr = m_ComponentMap.find(compID);
		if (compPtr != m_ComponentMap.end())
			return compPtr->second;

		// Create and insert new data into the m_ComponentMap map
		ComponentData* newCompData = new ComponentData;
		newCompData->size = sizeof(Comp);
		newCompData->moveData = [](unsigned char* src, unsigned char* dest)
			{ new (&dest[0]) Comp(std::move(*reinterpret_cast<Comp*>(src))); };

		m_ComponentMap.insert({ compID, newCompData });
		return newCompData;
	}
}