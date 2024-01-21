#pragma once
#include <string>
#include "entt/entt.hpp"
#include "ImGui/imgui.h"

#include "glm/vec3.hpp"

namespace LAG
{
	namespace Reflection
	{
		template <typename Type>
		static void SetWidgetType(const entt::meta_data& meta, const std::string& name, Type& value)
		{
			std::string errorMessage = "No Reflection type set up for " + name;
			ImGui::Text(errorMessage);
		}


		template <>
		static void SetWidgetType<int>(const entt::meta_data& meta, const std::string& name, int& value)
		{
			ImGui::DragInt(name.c_str(), &value);
		}

		template <>
		static void SetWidgetType<bool>(const entt::meta_data& meta, const std::string& name, bool& value)
		{
			ImGui::Checkbox(name.c_str(), &value);
		}

		template <>
		static void SetWidgetType<float>(const entt::meta_data& meta, const std::string& name, float& value)
		{
			ImGui::DragFloat(name.c_str(), &value, 0.1f);
		}

		template <>
		static void SetWidgetType<std::string>(const entt::meta_data& meta, const std::string& name, std::string& value)
		{
			ImGui::Text("STRING REFLECTION WIDGET NOT IMPLEMENTED");
		}

		template <>
		static void SetWidgetType<glm::vec3>(const entt::meta_data& meta, const std::string& name, glm::vec3& value)
		{
			ImGui::DragFloat3(name.c_str(), &value[0], 0.1f);
		}

	}
}