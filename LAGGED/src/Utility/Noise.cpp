#include "Noise.h"
#include "FastNoise/FastNoise.h"

#include "Logger.h"

namespace LAG::Noise
{
	GeneratedData Noise::GenerateNoiseData(const Properties& noiseProps, const glm::vec2& position, const glm::vec2& size, int surfaceSubdivision)
	{
		GeneratedData noiseData;
		glm::vec2 scale = size + 1.f;

		noiseData.size = scale;
		noiseData.data.resize(static_cast<size_t>((scale.x) * (scale.y)));

		if (size.x > 0 && size.y > 0)
		{
			float frequencyAdjustment = size.length() / static_cast<float>(surfaceSubdivision);

			//TODO: generator should be stored
			auto generator = FastNoise::NewFromEncodedNodeTree("GQANAAMAAACamRk/GwANAAIAAAC4HoU+BwAAuB4lQACF61E/AAAAgEAAAAAAQQAAAAAAASQAAgAAABwAARkADQACAAAAuB4FQP//AAAAj8I1QQB7FK4+AQ0AAgAAAMP1qED//wAAAJqZmT4AcT0KPwApXE9A");
			generator->GenUniformGrid2D(noiseData.data.data(), static_cast<int>(position.x), static_cast<int>(position.y), static_cast<int>(scale.x), static_cast<int>(scale.y), noiseProps.m_Frequency * frequencyAdjustment, noiseProps.m_Seed);
			if (noiseProps.m_Amplitude != 1.f)
			{
				for (int i = 0; i < noiseData.data.size(); i++)
					noiseData.data[i] *= noiseProps.m_Amplitude;
			}
		}
		else
			WARNING("Invalid noise map size");

		return noiseData;
	}

	Texture Noise::GeneratePreviewTexture(const Properties& noiseProperties, const glm::vec2& position, const glm::vec2& size, int surfaceSubdivision)
	{
		GeneratedData noiseData = GenerateNoiseData(noiseProperties, position, size, surfaceSubdivision);
		Texture texture(StringHash64(""));

		std::vector<unsigned char> ucharVec(noiseData.data.size());
		for (int i = 0; i < noiseData.data.size(); i++)
			ucharVec[i] = static_cast<unsigned char>(std::clamp<float>(noiseData.data[i] * UCHAR_MAX, 0.f, (float)(UCHAR_MAX)));
		
		texture.SetBuffer(&ucharVec.data()[0], noiseData.data.size(), static_cast<int>(noiseData.size.x), static_cast<int>(noiseData.size.y), LAG::TextureFormat::FORMAT_R);
		texture.Reload();

		return texture;
	}
}