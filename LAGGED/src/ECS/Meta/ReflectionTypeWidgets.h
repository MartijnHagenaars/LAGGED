#pragma once
#include <string>
#include "entt/entt.hpp"
#include "ImGui/imgui.h"

#include "glm/vec3.hpp"

namespace LAG
{
	namespace Reflection
	{
		inline const int REFLECTION_CHAR_ARRAY_SIZE = 256;
		static char reflectionWidgetTextBuffer[REFLECTION_CHAR_ARRAY_SIZE];

		template <typename T>
		[[maybe_unused]] static void SetWidgetType(const entt::meta_data& meta, const std::string& name, T& value)
		{
			std::string errorMessage = "No Reflection type set up for " + name;
			ImGui::Text(errorMessage);
		}

		template <>
		[[maybe_unused]] static void SetWidgetType<int>(const entt::meta_data& meta, const std::string& name, int& value)
		{
			ImGui::DragInt(name.c_str(), &value);
		}

		template <>
		[[maybe_unused]] static void SetWidgetType<bool>(const entt::meta_data& meta, const std::string& name, bool& value)
		{
			ImGui::Checkbox(name.c_str(), &value);
		}

		template <>
		[[maybe_unused]] static void SetWidgetType<float>(const entt::meta_data& meta, const std::string& name, float& value)
		{
			ImGui::DragFloat(name.c_str(), &value, 0.1f);
		}

		template <>
		[[maybe_unused]] static void SetWidgetType<std::string>(const entt::meta_data& meta, const std::string& name, std::string& value)
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
		[[maybe_unused]] static void SetWidgetType<glm::vec3>(const entt::meta_data& meta, const std::string& name, glm::vec3& value)
		{
			ImGui::DragFloat3(name.c_str(), &value[0], 0.1f);
		}

	}
}