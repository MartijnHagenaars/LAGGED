#pragma once
#include <string>
#include "entt/entt.hpp"
#include "ImGui/imgui.h"

#include "glm/vec3.hpp"

#include "Core/Engine.h"
#include "Core/Resources/Texture.h"
#include "Core/Resources/ResourceHandles.h"
#include "Core/Resources/ResourceManager.h"

namespace LAG
{
	namespace Reflection
	{
		inline const int REFLECTION_CHAR_ARRAY_SIZE = 256;
		static char reflectionWidgetTextBuffer[REFLECTION_CHAR_ARRAY_SIZE];

		////////////////////////
		// Drawing of widgets //
		////////////////////////

		template <typename T>
		[[maybe_unused]] static void DrawWidgetType(const entt::meta_data& meta, const std::string& name, T& value)
		{
			std::string errorMessage = "No Reflection type set up for " + name;
			ImGui::Text(errorMessage.c_str());
		}

		template <>
		[[maybe_unused]] static void DrawWidgetType<int>(const entt::meta_data& meta, const std::string& name, int& value)
		{
			ImGui::DragInt(name.c_str(), &value);
		}

		template <>
		[[maybe_unused]] static void DrawWidgetType<bool>(const entt::meta_data& meta, const std::string& name, bool& value)
		{
			ImGui::Checkbox(name.c_str(), &value);
		}

		template <>
		[[maybe_unused]] static void DrawWidgetType<float>(const entt::meta_data& meta, const std::string& name, float& value)
		{
			ImGui::DragFloat(name.c_str(), &value, 0.1f);
		}

		template <>
		[[maybe_unused]] static void DrawWidgetType<std::string>(const entt::meta_data& meta, const std::string& name, std::string& value)
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
		[[maybe_unused]] static void DrawWidgetType<glm::vec3>(const entt::meta_data& meta, const std::string& name, glm::vec3& value)
		{
			ImGui::DragFloat3(name.c_str(), &value[0], 0.1f);
		}

		template <>
		static void DrawWidgetType<TextureHandle>(const entt::meta_data& meta, const std::string& name, TextureHandle& value)
		{
			Texture* texture = GetEngine().GetResources()->GetResource<Texture>(value.m_TextureLookup.GetValue());
			if (texture != nullptr)
			{
				//Draw texture in ImGui window
				ImGui::Image(texture->GetEditorHandle(), ImVec2(
					std::clamp<float>(ImGui::GetWindowSize().x, 0.f, 256.f), 
					std::clamp<float>((ImGui::GetWindowSize().x / texture->GetHeight()) * texture->GetHeight(), 0.f, 256.f)
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



		////////////////////////
		// Loading of widgets //
		////////////////////////

		template <typename T>
		[[maybe_unused]] static void LoadWidgetType(const entt::meta_data& meta, const std::string& name, T& value)
		{
		}



		//////////////////////////
		// Unloading of widgets //
		//////////////////////////

		template <typename T>
		[[maybe_unused]] static void UnloadWidgetType(const entt::meta_data& meta, const std::string& name, T& value)
		{
		}
	}
}