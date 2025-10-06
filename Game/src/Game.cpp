#include "Game.h"
#include "Core/EntryPoint.h"

#include "Core/Input/Input.h"

#include "Core/Engine.h"
#include "Core/Resources/ResourceManager.h"

#include "Platform/RenderBackend.h"
#include "Platform/Resources/Model.h"

#include "ECS/Scene.h"
#include "ECS/Entity.h"
#include "ECS/Components/BasicComponents.h"
#include "ECS/Components/ModelComponent.h"
#include "ECS/Components/CameraComponent.h"
#include "ECS/Components/LightComponent.h"
#include "ECS/Components/TerrainComponents.h"

#include "ECS/Systems/TerrainSystems.h"

#include "Utility/Profiler.h"

#include "World/World.h"

ENTRY_APP(Game)

Game::Game()
{
}

Game::~Game()
{
}

template <typename T>
[[maybe_unused]] static void ReflFuncTest(LAG::EntityID id, T& value)
{
	INFO("No type found!");
}

template <>
[[maybe_unused]] static void ReflFuncTest<bool>(LAG::EntityID id, bool& value)
{
	INFO("Reflection function found! Reflecting a boolean, right? Or wrong??? ???????????");
}

template <>
[[maybe_unused]] static void ReflFuncTest<glm::vec3>(LAG::EntityID id, glm::vec3& value)
{
	INFO("Reflection function found! Reflecting a vec3! That's triple the floats!");
}

void Game::Initialize()
{
	LAG::Input::AddInputAction(LAG::Input::InputType::LAG_W, LAG::StringHash64("UpButton"));
	LAG::Input::AddInputAction(LAG::Input::InputType::LAG_S, LAG::StringHash64("DownButton"));
	LAG::Input::AddInputAction(LAG::Input::InputType::LAG_LMB, LAG::StringHash64("LMB"));

	LAG::GetRenderer()->SetSkyboxCubemap(LAG::StringHash64("res/Assets/Cubemaps/Sky_2k"));
	LAG::GetResourceManager()->AddResource<LAG::Model>(LAG::StringHash64("res/Assets/Models/Helmet/DamagedHelmet.gltf"));
	//LAG::GetResourceManager()->AddResource<LAG::Model>(LAG::StringHash64("res/Assets/Models/Avocado/Avocado.gltf"));
	//LAG::GetResourceManager()->AddResource<LAG::Model>(LAG::StringHash64("res/Assets/Models/BoomBox/BoomBox.gltf"));
	//LAG::GetResourceManager()->AddResource<LAG::Model>(LAG::StringHash64("res/Assets/Models/ChessGame/ABeautifulGame.gltf"));

	//Load an entity with a model and a transform
	LAG::Entity ent1 = LAG::GetEngine().GetScene()->AddEntity("Helmet");
	ent1.AddComponent<LAG::TransformComponent>()->SetPosition(glm::vec3(0.f, 0.f, 0.f));
	ent1.AddComponent<LAG::TransformComponent>()->SetScale(glm::vec3(1.f));
	//ent1.AddComponent<LAG::ModelComponent>("res/Assets/Models/ChessGame/ABeautifulGame.gltf");
	ent1.AddComponent<LAG::ModelComponent>("res/Assets/Models/Helmet/DamagedHelmet.gltf");
	ent1.AddComponent<LAG::SinWaveComponent>(6.f, 1.f);

	LAG::Entity ent2 = LAG::GetEngine().GetScene()->AddEntity("Light1");
	ent2.AddComponent<LAG::TransformComponent>()->SetPosition(glm::vec3(0.f, 0.f, -5.f));
	auto ent2Light = ent2.AddComponent<LAG::LightComponent>();
	ent2Light->lightAttenuation = 5.f;
	ent2Light->lightIntensity = 2.f;

	LAG::Entity ent3 = LAG::GetEngine().GetScene()->AddEntity("Light2");
	auto ent3Light = ent3.AddComponent<LAG::LightComponent>();
	ent3Light->lightAttenuation = 0.1f;
	ent3Light->lightIntensity = 2.f;
	ent3Light->lightColor = glm::vec3(0.f, 1.f, 0.f);
	ent3.AddComponent<LAG::TransformComponent>()->SetPosition(glm::vec3(-5.f, -2.f, -7.f));

	LAG::Entity ent4 = LAG::GetEngine().GetScene()->AddEntity("Light3");
	ent4.AddComponent<LAG::TransformComponent>()->SetPosition(glm::vec3(5.f, 40.f, -7.f));
	auto ent4Light = ent4.AddComponent<LAG::LightComponent>();
	ent4Light->lightAttenuation = 0.0001f;
	ent4Light->lightIntensity = 1.f;
	ent4Light->lightColor = glm::vec3(1.f, 1.f, 1.f);

	LAG::Entity ent5 = LAG::GetEngine().GetScene()->AddEntity("Camera");
	ent5.AddComponent<LAG::TransformComponent>()->SetPosition(glm::vec3(0.f, -2.f, 0.f));
	ent5.GetComponent<LAG::TransformComponent>()->SetRotation(glm::vec3(1.57f, 0.f, 0.f));
	ent5.AddComponent<LAG::CameraComponent>()->movementSpeed = 50.f;

	LAG::GetEngine().GetScene()->AddEntity("Empty Entity");

	m_World = new World(12, true);
	LAG::Scene* sc = LAG::GetEngine().GetScene();

	LAG::SceneReflect::RegisterFunc<bool>(LAG::StringToHash64("REFL_FUNC_TEST"), &ReflFuncTest);

	sc->ForEach<LAG::TransformComponent>([](LAG::EntityID entID, LAG::TransformComponent* transformCmp)
		{
			INFO("TransformComponent ForEach - {}", entID);
		});

	sc->ForEach<>([](LAG::EntityID entID)
		{
			INFO("Undefined ForEach - {}", entID);
		});

	LAG::EntityID reflEntityID = ent5.ID();
	for (LAG::ArchetypeView archIt : sc->Range())
	{
		if (archIt.Contains(reflEntityID))
		{
			for (LAG::ComponentView compIt : archIt.Types())
			{
				auto data = compIt.GetVoid(reflEntityID);
				std::any dataAny = compIt.ToAny(data);
				if (dataAny.type() == typeid(LAG::DefaultComponent*))
				{
					LAG::DefaultComponent* defComp = std::any_cast<LAG::DefaultComponent*>(dataAny);
					INFO("Component info: name({}), size({})", compIt.Name(), compIt.Size());
				}

				for (LAG::MemberView varIt : compIt.Members())
				{
					auto& props = varIt.Properties();
					std::any varAny = varIt.ToAny(varIt.GetVoid(reflEntityID));
					if (varAny.type() == typeid(std::string))
					{
						std::string str = std::any_cast<std::string>(varAny);
						INFO("String: {}", str);
					}

					const auto& name = props.displayName;

					varIt.InvokeFunc(LAG::StringToHash64("REFL_FUNC_TEST"));
				}
			}
		}
	}
}

void Game::Shutdown()
{
}

void Game::Update()
{
	LAG_PROFILE();

	m_World->Update();
}
