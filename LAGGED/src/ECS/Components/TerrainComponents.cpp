#include "TerrainComponents.h"

#include "ECS/Scene.h"
#include "ECS/Meta/ReflectionDefines.h"

namespace LAG
{
	bool SurfaceComponent::InitializeReflection()
	{
        auto factory = entt::meta<SurfaceComponent>();
        factory.type(entt::type_hash<SurfaceComponent>::value());

        factory.data<&SurfaceComponent::m_Surface>(entt::hashed_string("m_Surface")).prop(Reflection::DISPLAY_NAME, std::string("Surface"));
        factory.data<&SurfaceComponent::m_SurfaceTexture>(entt::hashed_string("m_SurfaceTexture")).prop(Reflection::DISPLAY_NAME, std::string("Texture"));

        return false;
	}

    bool ProceduralTerrainComponent::InitializeReflection()
    {
        auto factory = entt::meta<ProceduralTerrainComponent>();
        factory.type(entt::type_hash<ProceduralTerrainComponent>::value());

        factory.data<&ProceduralTerrainComponent::m_Surface>(entt::hashed_string("m_Surface")).prop(Reflection::DISPLAY_NAME, std::string("Surface"));
        factory.data<&ProceduralTerrainComponent::m_SurfaceTexture>(entt::hashed_string("m_SurfaceTexture")).prop(Reflection::DISPLAY_NAME, std::string("Texture"));
        factory.data<&ProceduralTerrainComponent::m_NoiseData>(entt::hashed_string("m_NoiseData")).prop(Reflection::DISPLAY_NAME, std::string("Noise Data"));


        return false;
    }

}