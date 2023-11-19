#version 330 core

out vec4 colorOut;

void main()
{
	vec3 baseColor = vec3(1.0, 0.0, 0.0);
	colorOut = vec4(baseColor, 1.0);
} 