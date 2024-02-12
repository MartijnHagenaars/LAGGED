#pragma once
#include <string>
#include <vector>
#include "Core/Resources/Texture.h"
#include "glm/vec2.hpp"

namespace LAG
{
	namespace Noise
	{
		struct Properties
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

		struct GeneratedData
		{
			std::vector<float> m_Data;
			glm::vec2 m_Size;
		};

		GeneratedData GenerateNoiseData(const Properties& noiseProperties, const glm::vec2& position, const glm::vec2& size, int surfaceSubdivision);

		Texture GeneratePreviewTexture(const Properties& noiseProperties, const glm::vec2& position, const glm::vec2& size, int surfaceSubdivision = 1);
	}
}