#pragma once
#include <string>
#include <vector>
#include "Core/Resources/Texture.h"
#include "glm/vec2.hpp"

namespace LAG
{
	struct NoiseProperties
	{
		int m_Seed = 0;
		float m_Frequency = 1.f;
		float m_Amplitude = 1.f;

		glm::vec2 m_NoisePosition;
		bool m_UseTransformPositionForNoise = true;

		std::string m_NodeTreeString = 
			"GQANAAIAAAC4HoU+BwAAuB4lQACF61E/ASQAAgAAABwAARkADQACAAAAuB4FQP//AAAAj8I1QQB7FK4+AQ0AAgAAAMP1qED//wAAAJqZmT4AcT0KPwAAAABA";

		Texture m_PreviewTexture;
	};

	namespace Noise
	{
		std::vector<float> GenerateNoiseData(const NoiseProperties& noiseProperties, const glm::vec2& position, const glm::vec2& size);

		Texture GeneratePreviewTexture(const NoiseProperties& noiseProperties, const glm::vec2& position, const glm::vec2& size);
	}
}