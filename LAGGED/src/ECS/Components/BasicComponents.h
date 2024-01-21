#pragma once
#include <string>
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"

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

		static bool InitializeReflection();
		static inline bool m_ReflectionState = InitializeReflection();
	};


	struct TransformComponent
	{
		TransformComponent();
		explicit TransformComponent(const glm::vec3& position) : 
			position(position)
		{}

		TransformComponent(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale) :
			position(position), rotation(rotation), scale(scale)
		{}

		void SetTransformMatrix(const glm::mat4 transformMat) { transform = transformMat; dirty = false; }
		const glm::mat4& GetTransformMatrix();

		void SetPosition(const glm::vec3& position) { this->position = position; dirty = true; }
		void SetRotation(const glm::vec3& rotation) { this->rotation = rotation; dirty = true; }
		void SetScale(const glm::vec3& scale) { this->scale = scale; dirty = true; }

		glm::vec3 GetPosition() const { return position; }
		glm::vec3 GetRotation() const { return rotation; }
		glm::vec3 GetScale() const { return scale; }

	private:
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;
		glm::mat4 transform;
		bool dirty = true;

		static bool InitializeReflection();
		static inline bool m_ReflectionState = InitializeReflection();
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