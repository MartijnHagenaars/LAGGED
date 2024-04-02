#include "Noise.h"
#include "FastNoise/FastNoise.h"

#include "Logger.h"

namespace LAG::Noise
{
	GeneratedData Noise::GenerateNoiseData(const Properties& noiseProps, const glm::vec2& position, const glm::vec2& size, int surfaceSubdivision)
	{
		GeneratedData noiseData;
		glm::vec2 scale = size + 1.f;

		noiseData.m_Size = scale;
		noiseData.m_Data.resize((scale.x) * (scale.y));

		if (size.x > 0 && size.y > 0)
		{
			float frequencyAdjustment = size.length() / static_cast<float>(surfaceSubdivision);

			//TODO: generator should be stored
			auto generator = FastNoise::NewFromEncodedNodeTree("GQANAAMAAACamRk/GwANAAIAAAC4HoU+BwAAuB4lQACF61E/AAAAgEAAAAAAQQAAAAAAASQAAgAAABwAARkADQACAAAAuB4FQP//AAAAj8I1QQB7FK4+AQ0AAgAAAMP1qED//wAAAJqZmT4AcT0KPwApXE9A");
			generator->GenUniformGrid2D(noiseData.m_Data.data(), static_cast<int>(position.x), static_cast<int>(position.y), scale.x, scale.y, noiseProps.m_Frequency * frequencyAdjustment, noiseProps.m_Seed);
			if (noiseProps.m_Amplitude != 1.f)
			{
				for (int i = 0; i < noiseData.m_Data.size(); i++)
					noiseData.m_Data[i] *= noiseProps.m_Amplitude;
			}
		}
		else
			Logger::Warning("Invalid noise map size");

		return noiseData;
	}

	Texture Noise::GeneratePreviewTexture(const Properties& noiseProperties, const glm::vec2& position, const glm::vec2& size, int surfaceSubdivision)
	{
		GeneratedData noiseData = GenerateNoiseData(noiseProperties, position, size, surfaceSubdivision);
		Texture texture(HashedString(""));

		texture.SetBuffer(noiseData.m_Data.data(), noiseData.m_Data.size(), noiseData.m_Size.x, noiseData.m_Size.y, LAG::TextureFormat::FORMAT_R);
		texture.Reload();

		return texture;
	}
}