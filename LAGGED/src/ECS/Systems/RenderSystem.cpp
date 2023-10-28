#include "Precomp.h"
#include "RenderSystem.h"

#include "ECS/Scene.h"
#include "ECS/Components/BasicComponents.h"
#include "ECS/Components/MeshComponent.h"
#include "Core/Engine.h"

void LAG::RenderSystem::Render()
{
	GetEngine().GetScene();
}
