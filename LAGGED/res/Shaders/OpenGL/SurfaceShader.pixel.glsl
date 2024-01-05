#version 330 core

in vec3 fragPosition;
in vec3 normal;

out vec4 colorOut;

vec3 CalculateDiffuse(vec3 lightPosition, vec3 lightColor)
{
	vec3 lightDirection = normalize(lightPosition - fragPosition);
	float diffuseIntensity = max(0.0, dot(normal, lightDirection));
	return diffuseIntensity * lightColor;
}

void main()
{

	vec3 baseColor = vec3(1.0, 0.0, 0.0) * CalculateDiffuse(vec3(0.0, 0.0, 0.0), vec3(1.0, 1.0, 1.0));
	//vec3 baseColor = vec3(0.0, 1.0, 0.0);
	colorOut = vec4(baseColor, 1.0);
} 