#version 330 core

in vec3 fragPosition;
in vec3 normal;
in vec2 texCoord;

out vec4 colorOut;

uniform sampler2D a_Texture1;

//Lighting data
uniform bool a_UseLight;

struct PointLightData
{			
	vec3 a_LightPosition;
	vec3 a_LightColor;
	float a_LightIntensity;
	float a_LightAttenuation;
};
#define TOTAL_POINT_LIGHTS 3
uniform PointLightData a_PointLightData[TOTAL_POINT_LIGHTS];

vec3 CalculateDiffuse(vec3 lightPosition, vec3 lightColor)
{
	vec3 lightDirection = normalize(lightPosition - fragPosition);
	float diffuseIntensity = max(0.0, dot(normal, lightDirection));
	return diffuseIntensity * lightColor;
}

float CalculateAttenuation(vec3 lightPosition, float lightIntensity, float lightAttenuation)
{
	float lightDistance = distance(fragPosition, lightPosition);
	return lightIntensity * (1.0 / (1.0 + lightAttenuation * lightDistance * lightDistance));
}

void main()
{
	vec3 lightCalculation = vec3(0.0);
	if(a_UseLight)
	{
		for(int i = 0; i < TOTAL_POINT_LIGHTS; i++)
			lightCalculation += (CalculateDiffuse(a_PointLightData[i].a_LightPosition, a_PointLightData[i].a_LightColor)) * CalculateAttenuation(a_PointLightData[i].a_LightPosition, a_PointLightData[i].a_LightIntensity, a_PointLightData[i].a_LightAttenuation);
	}
	else lightCalculation = vec3(1.0);

    colorOut = texture(a_Texture1, texCoord) * vec4(lightCalculation, 1.f);
    //colorOut = vec4(normal, 1.f); //Draw normal
} 