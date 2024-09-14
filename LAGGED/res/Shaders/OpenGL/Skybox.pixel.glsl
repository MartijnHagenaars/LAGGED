#version 330 core

in vec3 texCoord;
out vec4 colorOut;

uniform samplerCube a_Skybox;

void main()
{
	colorOut = texture(a_Skybox, texCoord);
}