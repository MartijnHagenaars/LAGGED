#include "Noise.h"
#include "FastNoise/FastNoise.h"

#include "Logger.h"

namespace LAG
{
	std::vector<float> Noise::GenerateNoiseData(const NoiseProperties& noiseProps, const glm::vec2& position, const glm::vec2& size)
	{
		std::vector<float> noiseData(size.x * size.y);

		if (size.x > 0 && size.y > 0)
		{
			//TODO: generator should be stored
			auto generator = FastNoise::NewFromEncodedNodeTree("GQANAAIAAAC4HoU+BwAAuB4lQACF61E/ASQAAgAAABwAARkADQACAAAAuB4FQP//AAAAj8I1QQB7FK4+AQ0AAgAAAMP1qED//wAAAJqZmT4AcT0KPwAAAABA");
			generator->GenUniformGrid2D(noiseData.data(), position.x, position.y, size.x, size.y, noiseProps.m_Frequency, noiseProps.m_Seed);
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
		std::vector<float> noiseData = GenerateNoiseData(noiseProperties, position, size);
		Texture texture(HashedString(""));
		texture.SetBuffer(noiseData.data(), noiseData.size(), LAG::TextureFormat::FORMAT_R);
		texture.Load();
		return texture;
	}
}