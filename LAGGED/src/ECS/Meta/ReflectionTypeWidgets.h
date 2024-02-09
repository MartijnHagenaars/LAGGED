#pragma once
#include <string>
#include "entt/entt.hpp"
#include "ImGui/imgui.h"

#include "glm/vec3.hpp"

#include "ECS/Entity.h"
#include "Core/Engine.h"
#include "Core/Resources/Texture.h"
#include "Core/Resources/ResourceHandles.h"
#include "Core/Resources/ResourceManager.h"

#include "ECS/Components/BasicComponents.h"
#include "Utility/Noise.h"

namespace LAG
{
	namespace Reflection
	{
		const float IMAGE_MAX_SIZE = 360.f;
		inline const int REFLECTION_CHAR_ARRAY_SIZE = 256;
		static char reflectionWidgetTextBuffer[REFLECTION_CHAR_ARRAY_SIZE];

		////////////////////////
		// Drawing of widgets //
		////////////////////////

		//DrawWidgetType<T>(entity, value.cast<T&>(), name);

		template <typename T>
		[[maybe_unused]] static void DrawWidgetType(LAG::Entity* entity, T& value, const std::string& name)
		{
			std::string errorMessage = "No Reflection type set up for " + name;
			ImGui::Text(errorMessage.c_str());
		}

		template <>
		[[maybe_unused]] static void DrawWidgetType<int>(LAG::Entity* entity, int& value, const std::string& name)
		{
			ImGui::DragInt(name.c_str(), &value);
		}

		template <>
		[[maybe_unused]] static void DrawWidgetType<bool>(LAG::Entity* entity, bool& value, const std::string& name)
		{
			ImGui::Checkbox(name.c_str(), &value);
		}

		template <>
		[[maybe_unused]] static void DrawWidgetType<float>(LAG::Entity* entity, float& value, const std::string& name)
		{
			ImGui::DragFloat(name.c_str(), &value, 0.1f);
		}

		template <>
		[[maybe_unused]] static void DrawWidgetType<std::string>(LAG::Entity* entity, std::string& value, const std::string& name)
		{
			//Clear the text buffer and copy the current value into it
			memset(reflectionWidgetTextBuffer, 0, REFLECTION_CHAR_ARRAY_SIZE);
			value.copy(reflectionWidgetTextBuffer, REFLECTION_CHAR_ARRAY_SIZE);

			if(ImGui::InputText(name.c_str(), reflectionWidgetTextBuffer, REFLECTION_CHAR_ARRAY_SIZE, ImGuiInputTextFlags_EnterReturnsTrue))
			{
				value = reflectionWidgetTextBuffer;
			}
		}

		template <>
		[[maybe_unused]] static void DrawWidgetType<glm::vec3>(LAG::Entity* entity, glm::vec3& value, const std::string& name)
		{
			ImGui::DragFloat3(name.c_str(), &value[0], 0.1f);
		}

		template <>
		static void DrawWidgetType<TextureHandle>(LAG::Entity* entity, TextureHandle& value, const std::string& name)
		{
			Texture* texture = GetEngine().GetResources()->GetResource<Texture>(value.m_TextureLookup.GetValue());
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

			if (ImGui::BeginCombo("Textures", value.m_TextureLookup.GetValue() != 0 ? value.m_TextureLookup.GetString().c_str() : "No texture selected..."))
			{
				std::vector<HashedString> textures = GetEngine().GetResources()->GetResourceNames<Texture>();
				for (int i = 0; i < textures.size(); i++)
				{
					const bool isSelected = (texture != nullptr) && (texture->GetPath().GetValue() == textures[i].GetValue());
					if (ImGui::Selectable(textures[i].GetString().c_str(), isSelected))
					{
						value.m_TextureLookup = textures[i];
					}
				}
				ImGui::EndCombo();
			}
		}

		template <>
		[[maybe_unused]] static void DrawWidgetType<NoiseProperties>(LAG::Entity* entity, NoiseProperties& value, const std::string& name)
		{
			ImGui::DragFloat("Amplitude", &value.m_Amplitude, 0.05f);
			ImGui::DragFloat("Frequency", &value.m_Frequency, 0.5f, -1024.f, 1024.f, "%.3f", ImGuiSliderFlags_Logarithmic);
			ImGui::DragInt("Seed", &value.m_Seed, 1.f);

			//Texture preview functionality
			if(value.m_PreviewTexture.IsLoaded())
				ImGui::Image(value.m_PreviewTexture.GetEditorHandle(), ImVec2(
					std::clamp<float>(ImGui::GetWindowSize().x, 0.f, IMAGE_MAX_SIZE),
					std::clamp<float>((ImGui::GetWindowSize().x / value.m_PreviewTexture.GetHeight()) * value.m_PreviewTexture.GetHeight(), 0.f, IMAGE_MAX_SIZE)));
			if (ImGui::Button("Preview"))
			{
				glm::vec2 position = glm::vec2(0.f, 0.f);
				glm::vec3 planeScale = entity->GetComponent<TransformComponent>()->GetScale();
				value.m_PreviewTexture = Noise::GeneratePreviewTexture(value, position, glm::vec2(planeScale.x, planeScale.z));
			}
			ImGui::SameLine();
			ImGui::Button("Apply");
		}



		////////////////////////
		// Loading of widgets //
		////////////////////////

		template <typename T>
		[[maybe_unused]] static void LoadWidgetType(LAG::Entity* entity, T& value, const std::string& name)
		{
		}



		//////////////////////////
		// Unloading of widgets //
		//////////////////////////

		template <typename T>
		[[maybe_unused]] static void UnloadWidgetType(LAG::Entity* entity, T& value, const std::string& name)
		{
		}
	}
}