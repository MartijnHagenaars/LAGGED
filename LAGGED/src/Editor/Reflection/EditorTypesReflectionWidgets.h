#pragma once
#include <glm/vec3.hpp>
#include <ImGui/imgui.h>
#include <string>

#include "Core/Engine.h"
#include "Core/Resources/ResourceHandles.h"
#include "Core/Resources/ResourceManager.h"

#include "ECS/Systems/TerrainSystems.h"
#include "ECS/Reflection/SceneReflect.h"

#include "Platform/Resources/Model.h"
#include "Utility/Noise.h"

namespace LAG
{
	constexpr Hash64 HANDLE_EDITOR_WIDGET_FUNC = StringToHash64("HANDLE_EDITOR_WIDGET");

	namespace Reflection
	{
		const float IMAGE_MAX_SIZE = 360.f;
		const int REFLECTION_CHAR_ARRAY_SIZE = 256;

		static char g_StringTextBuffer[REFLECTION_CHAR_ARRAY_SIZE];
		
		////////////////////////
		// Drawing of widgets //
		////////////////////////

		template <typename T>
		[[maybe_unused]] static void DrawWidgetType(LAG::EntityID entityID, T* value, const std::string& name)
		{
			std::string errorMessage = "No Reflection type set up for " + name;
			ImGui::Text(errorMessage.c_str());
		}

		template <>
		[[maybe_unused]] static void DrawWidgetType<int>(LAG::EntityID entityID, int* value, const std::string& name)
		{
			ImGui::DragInt(name.c_str(), value);
		}

		template <>
		[[maybe_unused]] static void DrawWidgetType<bool>(LAG::EntityID entityID, bool* value, const std::string& name)
		{
			ImGui::Checkbox(name.c_str(), value);
		}

		template <>
		[[maybe_unused]] static void DrawWidgetType<float>(LAG::EntityID entityID, float* value, const std::string& name)
		{
			ImGui::DragFloat(name.c_str(), value, 0.1f);
		}

		template <>
		[[maybe_unused]] static void DrawWidgetType<std::string>(LAG::EntityID entityID, std::string* value, const std::string& name)
		{
			//Clear the text buffer and copy the current value into it
			memset(g_StringTextBuffer, 0, REFLECTION_CHAR_ARRAY_SIZE);
			value->copy(g_StringTextBuffer, REFLECTION_CHAR_ARRAY_SIZE);
			if (ImGui::InputText(name.c_str(), g_StringTextBuffer, REFLECTION_CHAR_ARRAY_SIZE, ImGuiInputTextFlags_EnterReturnsTrue))
			{
				value->assign(g_StringTextBuffer);
			}
		}

		template <>
		[[maybe_unused]] static void DrawWidgetType<glm::vec3>(LAG::EntityID entityID, glm::vec3* value, const std::string& name)
		{
			ImGui::DragFloat3(name.c_str(), &value->x, 0.1f);
		}

		template <>
		static void DrawWidgetType<TextureHandle>(LAG::EntityID entityID, TextureHandle* value, const std::string& name)
		{
			Texture* texture = nullptr;
			if (ImGui::BeginCombo("Textures", value->m_TextureLookup.Value() != 0 ? value->m_TextureLookup.String().c_str() : "No texture selected..."))
			{
				std::vector<StringHash64> textures = GetEngine().GetResources()->GetResourceNames<Texture>();
				for (int i = 0; i < textures.size(); i++)
				{
					const bool isSelected = (texture != nullptr) && (texture->GetPath().Value() == textures[i].Value());
					if (ImGui::Selectable(textures[i].String().c_str(), isSelected))
					{
						value->m_TextureLookup = textures[i];
					}
				}
				ImGui::EndCombo();
			}

			if (!value->m_TextureLookup.String().empty())
				texture = GetEngine().GetResources()->GetResource<Texture>(value->m_TextureLookup.Value());
			if (texture != nullptr)
			{
				//Draw texture in ImGui window
				ImGui::Image(texture->GetEditorHandle(), ImVec2(
					std::clamp<float>(ImGui::GetWindowSize().x, 0.f, IMAGE_MAX_SIZE),
					std::clamp<float>((ImGui::GetWindowSize().x / texture->GetHeight()) * texture->GetHeight(), 0.f, IMAGE_MAX_SIZE)
				));
			}
			else
			{
				ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), "No texture assigned.");
			}
		}

		template <>
		static void DrawWidgetType<ModelHandle>(LAG::EntityID entityID, ModelHandle* value, const std::string& name)
		{
			Model* model = nullptr;
			if (ImGui::BeginCombo("Models", value->m_ModelLookup.Value() != 0 ? value->m_ModelLookup.String().c_str() : "No model selected..."))
			{
				std::vector<StringHash64> models = GetEngine().GetResources()->GetResourceNames<Model>();
				for (int i = 0; i < models.size(); i++)
				{
					const bool isSelected = (model != nullptr) && (model->GetPath().Value() == models[i].Value());
					if (ImGui::Selectable(models[i].String().c_str(), isSelected))
					{
						value->m_ModelLookup = models[i];
					}
				}
				if (ImGui::Selectable("Other..."))
				{
					printf("other selected...");
				}

				ImGui::EndCombo();
			}
		}

		template <>
		[[maybe_unused]] static void DrawWidgetType<Noise::Properties>(LAG::EntityID entityID, Noise::Properties* value, const std::string& name)
		{
			ImGui::DragFloat("Amplitude", &value->m_Amplitude, 0.05f);
			ImGui::DragFloat("Frequency", &value->m_Frequency, 0.5f, -1024.f, 1024.f, "%.3f", ImGuiSliderFlags_Logarithmic);
			ImGui::DragInt("Seed", &value->m_Seed, 1.f);

			ImGui::Checkbox("Use Transform component for Noise position", &value->m_UseTransformPositionForNoise);
			if (value->m_UseTransformPositionForNoise)
				ImGui::BeginDisabled();
			ImGui::DragFloat2("Noise positions", &value->m_NoisePosition[0], 0.25f, -FLT_MAX, FLT_MAX, "%.3f");
			if (value->m_UseTransformPositionForNoise)
				ImGui::EndDisabled();

			////Texture preview functionality
			//if (value.m_PreviewTexture.IsLoaded())
			//	ImGui::Image(value.m_PreviewTexture.GetEditorHandle(), ImVec2(
			//		std::clamp<float>(ImGui::GetWindowSize().x, 0.f, IMAGE_MAX_SIZE),
			//		std::clamp<float>((ImGui::GetWindowSize().x / value.m_PreviewTexture.GetHeight()) * value.m_PreviewTexture.GetHeight(), 0.f, IMAGE_MAX_SIZE)));
			//if (ImGui::Button("Preview"))
			//{
			//	//TODO: This looks terrible
			//	glm::vec2 position = value.m_UseTransformPositionForNoise ?
			//		glm::vec2(entity->GetComponent<TransformComponent>()->GetPosition().x, entity->GetComponent<TransformComponent>()->GetPosition().z) :
			//		value.m_NoisePosition;

			//	int surfaceSubdivisions = entity->GetComponent<ProceduralSurfaceComponent>()->surfaceSubdivisions;
			//	glm::vec3 planeScale = entity->GetComponent<TransformComponent>()->GetScale();
			//	value.m_PreviewTexture = Noise::GeneratePreviewTexture(value, position, glm::vec2(planeScale.x, planeScale.z), surfaceSubdivisions);
			//}

			//ImGui::SameLine();
			//if (ImGui::Button("Apply"))
			//	SurfaceSystems::GenerateNoiseSurface(entity);
		}


		////////////////////////
		// Loading of widgets //
		////////////////////////

		template <typename T>
		[[maybe_unused]] static void LoadWidgetType(LAG::EntityID entityID, T* value, const std::string& name)
		{
		}



		//////////////////////////
		// Unloading of widgets //
		//////////////////////////

		template <typename T>
		[[maybe_unused]] static void UnloadWidgetType(LAG::EntityID entityID, T* value, const std::string& name)
		{
		}

		template <>
		[[maybe_unused]] static void UnloadWidgetType<Noise::Properties>(LAG::EntityID entityID, Noise::Properties* value, const std::string& name)
		{
			//if (value.m_PreviewTexture.IsLoaded())
			//	value.m_PreviewTexture.Unload();
		}

		//////////////////////////////////////////////////
		//     Registring editor reflection widgets     //
		// (should probably be moved to somewhere else) //
		//////////////////////////////////////////////////
		namespace InternalSetup
		{
			template <typename T>
			static void SetupEditorTypesReflection()
			{
				SceneReflect::RegisterFunc<T, &LAG::Reflection::DrawWidgetType<T>>(HANDLE_EDITOR_WIDGET_FUNC);
			}

			static bool InitializeReflectionTypes()
			{
				SetupEditorTypesReflection<int>();
				SetupEditorTypesReflection<bool>();
				SetupEditorTypesReflection<float>();
				SetupEditorTypesReflection<std::string>();

				SetupEditorTypesReflection<glm::vec3>();

				SetupEditorTypesReflection<LAG::ModelHandle>();
				SetupEditorTypesReflection<LAG::TextureHandle>();
				SetupEditorTypesReflection<LAG::Noise::Properties>();

				return true;
			}
			static inline bool m_ReflectionTypeState = InitializeReflectionTypes();
		}
	}
}