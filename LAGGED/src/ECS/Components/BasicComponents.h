#pragma once
#include <string>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace LAG
{
	struct DefaultComponent
	{
		DefaultComponent() = default;
		explicit DefaultComponent(std::string name) :
			name(name), visible(true)
		{
		};

		std::string name;
		bool visible = true;
		
		static bool InitializeReflection();
		static inline bool m_ReflectionState = InitializeReflection();
	};


	struct TransformComponent
	{
		TransformComponent();
		explicit TransformComponent(const glm::vec3& translation);
		TransformComponent(const glm::vec3& translation, const glm::vec3& rotation, const glm::vec3& scale);

		void SetTransformMatrix(const glm::mat4 transformMat);
		const glm::mat4& GetTransformMatrix();

		void SetPosition(const glm::vec3& translation) { this->translation = translation; m_Dirty = true; }
		void SetRotation(const glm::vec3& rotation) { this->rotation = rotation; m_Dirty = true; }
		void SetScale(const glm::vec3& scale) { this->scale = scale; m_Dirty = true; }

		glm::vec3 GetPosition() const { return translation; }
		glm::vec3 GetRotation() const { return rotation; }
		glm::vec3 GetScale() const { return scale; }

	private:
		glm::vec3 translation;
		glm::vec3 rotation;
		glm::vec3 scale;
		glm::mat4 transform;
		bool m_Dirty = true;

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

		static bool InitializeReflection();
		static inline bool m_ReflectionState = InitializeReflection();
	};
}