#pragma once
#include <string>
#include "glm/vec3.hpp"

namespace LAG
{
	struct DefaultComponent
	{
		explicit DefaultComponent(std::string name) :
			name(name), visible(true)
		{
		};

		std::string name;
		bool visible;

		static int InitReflection();
		static inline int m_ReflectionID = InitReflection();
	};


	struct TransformComponent
	{
		TransformComponent() = default;
		explicit TransformComponent(const glm::vec3& position) : 
			position(position)
		{}

		TransformComponent(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale) :
			position(position), rotation(rotation), scale(scale)
		{}

		glm::vec3 position = glm::vec3(0.f);
		glm::vec3 rotation = glm::vec3(0.f);
		glm::vec3 scale = glm::vec3(1.f);

		static int InitReflection();
		static inline int m_ReflectionID = InitReflection();
	};

	struct SinWaveComponent
	{
		SinWaveComponent() = default;
		SinWaveComponent(float amplitude, float frequency) :
			amplitude(amplitude), frequency(frequency)
		{}

		float sinValue = 0.f;
		float time = 0.f;

		float amplitude = 1.f;
		float frequency = 1.f;
	};
}