
namespace LAG
{
	template<typename Comp, typename ...Args>
	inline Comp* Scene::AddComponent(const EntityID entityID, Args && ...args)
	{
		ComponentID newCompID = GetComponentID<Comp>();

		return nullptr;
	}
	template<typename Comp>
	inline const ComponentID Scene::GetComponentID()
	{
		static const ComponentID compID = s_ComponentCounter++;
		return compID;
	}
}