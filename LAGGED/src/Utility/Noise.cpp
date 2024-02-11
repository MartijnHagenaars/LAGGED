#include "Noise.h"
#include "FastNoise/FastNoise.h"

#include "Logger.h"

namespace LAG
{
	std::vector<float> Noise::GenerateNoiseData(const NoiseProperties& noiseProps, const glm::vec2& position, const glm::vec2& size, int surfaceSubdivision)
	{
		std::vector<float> noiseData(size.x * size.y);

		if (size.x > 0 && size.y > 0)
		{
			float frequencyAdjustment = size.length() / static_cast<float>(surfaceSubdivision);

			//TODO: generator should be stored
			auto generator = FastNoise::NewFromEncodedNodeTree("GQANAAIAAAC4HoU+BwAAuB4lQACF61E/ASQAAgAAABwAARkADQACAAAAuB4FQP//AAAAj8I1QQB7FK4+AQ0AAgAAAMP1qED//wAAAJqZmT4AcT0KPwAAAABA");
			generator->GenUniformGrid2D(noiseData.data(), position.x, position.y, size.x, size.y, noiseProps.m_Frequency * frequencyAdjustment, noiseProps.m_Seed);
			if (noiseProps.m_Amplitude != 1.f)
			{
				for (int i = 0; i < noiseData.size(); i++)
					noiseData[i] *= noiseProps.m_Amplitude;
			}
		}
		else
			Logger::Warning("Invalid noise map size");

		return noiseData;
	}

	Texture Noise::GeneratePreviewTexture(const NoiseProperties& noiseProperties, const glm::vec2& position, const glm::vec2& size)
	{
		std::vector<float> noiseData = GenerateNoiseData(noiseProperties, position, size, 1);
		Texture texture(HashedString(""));

		texture.SetBuffer(noiseData.data(), noiseData.size(), size.x, size.y, LAG::TextureFormat::FORMAT_R);
		texture.Reload();

		return texture;
	}
}